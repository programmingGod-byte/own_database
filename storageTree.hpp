#ifndef __BTREE
#define __BTREE

#include <iostream>
#include <vector>
#include <algorithm>
#include <mutex>
#include <shared_mutex>
#include <memory>

template<typename K, typename V>
class BPlusTree {
private:
    static const int DEGREE = 100;
    static const int MAX_KEYS = 2 * DEGREE - 1;
    static const int MIN_KEYS = DEGREE - 1;

    struct Node {
        std::vector<K> keys;
        std::vector<V> values;  
        
        std::vector<Node*> children;  
        bool is_leaf;
        Node* next;  
        Node* parent;
        
        // Node-level mutex for fine-grained locking
        mutable std::shared_mutex mutex;

        Node(bool leaf = false) : is_leaf(leaf), next(nullptr), parent(nullptr) {}

        ~Node() {
            if (!is_leaf) {
                for (Node* child : children) {
                    delete child;
                }
            }
        }
    };

    Node* root;
    
    // Tree-level mutex for structural changes
    mutable std::shared_mutex tree_mutex;

    Node* find_leaf(const K& key) {
        std::shared_lock<std::shared_mutex> tree_lock(tree_mutex);
        
        Node* node = root;
        while (!node->is_leaf) {
            std::shared_lock<std::shared_mutex> node_lock(node->mutex);
            int i = 0;
            while (i < node->keys.size() && key >= node->keys[i]) {
                i++;
            }
            Node* next_node = node->children[i];
            node_lock.unlock();
            node = next_node;
        }
        return node;
    }

    void insert_into_leaf(Node* leaf, const K& key, const V& value) {
        std::unique_lock<std::shared_mutex> leaf_lock(leaf->mutex);
        
        auto it = std::lower_bound(leaf->keys.begin(), leaf->keys.end(), key);
        int pos = it - leaf->keys.begin();

        if (it != leaf->keys.end() && *it == key) {
            // Key already exists, update value
            leaf->values[pos] = value;
        } else {
            // Insert new key-value pair
            leaf->keys.insert(it, key);
            leaf->values.insert(leaf->values.begin() + pos, value);
        }
    }

    void split_leaf(Node* leaf) {
        std::unique_lock<std::shared_mutex> tree_lock(tree_mutex);
        std::unique_lock<std::shared_mutex> leaf_lock(leaf->mutex);
        
        Node* new_leaf = new Node(true);
        int mid = (leaf->keys.size() + 1) / 2;

        new_leaf->keys.assign(leaf->keys.begin() + mid, leaf->keys.end());
        new_leaf->values.assign(leaf->values.begin() + mid, leaf->values.end());

        leaf->keys.resize(mid);
        leaf->values.resize(mid);

        new_leaf->next = leaf->next;
        leaf->next = new_leaf;

        new_leaf->parent = leaf->parent;

        K promote_key = new_leaf->keys[0];
        
        leaf_lock.unlock();
        insert_into_parent(leaf, promote_key, new_leaf);
    }

    void insert_into_parent(Node* left, const K& key, Node* right) {
        if (left == root) {
            // Create new root
            Node* new_root = new Node(false);
            new_root->keys.push_back(key);
            new_root->children.push_back(left);
            new_root->children.push_back(right);
            left->parent = new_root;
            right->parent = new_root;
            root = new_root;
            return;
        }

        Node* parent = left->parent;
        std::unique_lock<std::shared_mutex> parent_lock(parent->mutex);

        auto it = std::lower_bound(parent->keys.begin(), parent->keys.end(), key);
        int pos = it - parent->keys.begin();

        parent->keys.insert(it, key);
        parent->children.insert(parent->children.begin() + pos + 1, right);
        right->parent = parent;

        bool needs_split = parent->keys.size() > MAX_KEYS;
        parent_lock.unlock();

        if (needs_split) {
            split_internal(parent);
        }
    }

    void split_internal(Node* node) {
        std::unique_lock<std::shared_mutex> node_lock(node->mutex);
        
        Node* new_node = new Node(false);
        int mid = node->keys.size() / 2;
        K promote_key = node->keys[mid];

        new_node->keys.assign(node->keys.begin() + mid + 1, node->keys.end());
        new_node->children.assign(node->children.begin() + mid + 1, node->children.end());

        for (Node* child : new_node->children) {
            std::unique_lock<std::shared_mutex> child_lock(child->mutex);
            child->parent = new_node;
        }

        node->keys.resize(mid);
        node->children.resize(mid + 1);

        new_node->parent = node->parent;

        node_lock.unlock();
        insert_into_parent(node, promote_key, new_node);
    }

    bool delete_from_leaf(Node* leaf, const K& key) {
        std::unique_lock<std::shared_mutex> leaf_lock(leaf->mutex);
        
        auto it = std::find(leaf->keys.begin(), leaf->keys.end(), key);
        if (it == leaf->keys.end()) {
            return false;  // Key not found
        }

        int pos = it - leaf->keys.begin();
        leaf->keys.erase(it);
        leaf->values.erase(leaf->values.begin() + pos);

        bool needs_rebalance = (leaf != root && leaf->keys.size() < MIN_KEYS);
        leaf_lock.unlock();

        if (needs_rebalance) {
            std::unique_lock<std::shared_mutex> tree_lock(tree_mutex);
            handle_underflow(leaf);
        }

        return true;
    }

    void handle_underflow(Node* node) {
        Node* parent = node->parent;
        if (!parent) return;  // Root node

        std::unique_lock<std::shared_mutex> parent_lock(parent->mutex);
        
        int pos = 0;
        while (pos < parent->children.size() && parent->children[pos] != node) {
            pos++;
        }

        // Try to borrow from left sibling
        if (pos > 0) {
            Node* left_sibling = parent->children[pos - 1];
            std::unique_lock<std::shared_mutex> left_lock(left_sibling->mutex);
            
            if (left_sibling->keys.size() > MIN_KEYS) {
                borrow_from_left(node, left_sibling, pos - 1);
                return;
            }
        }

        // Try to borrow from right sibling
        if (pos < parent->children.size() - 1) {
            Node* right_sibling = parent->children[pos + 1];
            std::unique_lock<std::shared_mutex> right_lock(right_sibling->mutex);
            
            if (right_sibling->keys.size() > MIN_KEYS) {
                borrow_from_right(node, right_sibling, pos);
                return;
            }
        }

        // Merge with sibling
        if (pos > 0) {
            merge_with_left(node, parent->children[pos - 1], pos - 1);
        } else {
            merge_with_right(node, parent->children[pos + 1], pos);
        }
    }

    void borrow_from_left(Node* node, Node* left_sibling, int parent_key_pos) {
        Node* parent = node->parent;
        std::unique_lock<std::shared_mutex> node_lock(node->mutex);

        if (node->is_leaf) {
            // Borrow from left sibling (leaf)
            K borrowed_key = left_sibling->keys.back();
            V borrowed_value = left_sibling->values.back();

            left_sibling->keys.pop_back();
            left_sibling->values.pop_back();

            node->keys.insert(node->keys.begin(), borrowed_key);
            node->values.insert(node->values.begin(), borrowed_value);

            parent->keys[parent_key_pos] = borrowed_key;
        } else {
            // Borrow from left sibling (internal)
            K parent_key = parent->keys[parent_key_pos];
            K borrowed_key = left_sibling->keys.back();
            Node* borrowed_child = left_sibling->children.back();

            left_sibling->keys.pop_back();
            left_sibling->children.pop_back();

            node->keys.insert(node->keys.begin(), parent_key);
            node->children.insert(node->children.begin(), borrowed_child);
            
            std::unique_lock<std::shared_mutex> child_lock(borrowed_child->mutex);
            borrowed_child->parent = node;

            parent->keys[parent_key_pos] = borrowed_key;
        }
    }

    void borrow_from_right(Node* node, Node* right_sibling, int parent_key_pos) {
        Node* parent = node->parent;
        std::unique_lock<std::shared_mutex> node_lock(node->mutex);

        if (node->is_leaf) {
            // Borrow from right sibling (leaf)
            K borrowed_key = right_sibling->keys.front();
            V borrowed_value = right_sibling->values.front();

            right_sibling->keys.erase(right_sibling->keys.begin());
            right_sibling->values.erase(right_sibling->values.begin());

            node->keys.push_back(borrowed_key);
            node->values.push_back(borrowed_value);

            parent->keys[parent_key_pos] = right_sibling->keys.front();
        } else {
            // Borrow from right sibling (internal)
            K parent_key = parent->keys[parent_key_pos];
            K borrowed_key = right_sibling->keys.front();
            Node* borrowed_child = right_sibling->children.front();

            right_sibling->keys.erase(right_sibling->keys.begin());
            right_sibling->children.erase(right_sibling->children.begin());

            node->keys.push_back(parent_key);
            node->children.push_back(borrowed_child);
            
            std::unique_lock<std::shared_mutex> child_lock(borrowed_child->mutex);
            borrowed_child->parent = node;

            parent->keys[parent_key_pos] = borrowed_key;
        }
    }

    void merge_with_left(Node* node, Node* left_sibling, int parent_key_pos) {
        Node* parent = node->parent;
        std::unique_lock<std::shared_mutex> node_lock(node->mutex);
        std::unique_lock<std::shared_mutex> left_lock(left_sibling->mutex);

        if (node->is_leaf) {
            left_sibling->keys.insert(left_sibling->keys.end(), node->keys.begin(), node->keys.end());
            left_sibling->values.insert(left_sibling->values.end(), node->values.begin(), node->values.end());
            left_sibling->next = node->next;
        } else {
            // Merge internal nodes
            left_sibling->keys.push_back(parent->keys[parent_key_pos]);
            left_sibling->keys.insert(left_sibling->keys.end(), node->keys.begin(), node->keys.end());
            left_sibling->children.insert(left_sibling->children.end(), node->children.begin(), node->children.end());

            for (Node* child : node->children) {
                std::unique_lock<std::shared_mutex> child_lock(child->mutex);
                child->parent = left_sibling;
            }
        }

        parent->keys.erase(parent->keys.begin() + parent_key_pos);
        parent->children.erase(parent->children.begin() + parent_key_pos + 1);

        node_lock.unlock();
        left_lock.unlock();
        delete node;

        if (parent != root && parent->keys.size() < MIN_KEYS) {
            handle_underflow(parent);
        } else if (parent == root && parent->keys.empty()) {
            root = left_sibling;
            left_sibling->parent = nullptr;
            delete parent;
        }
    }

    void merge_with_right(Node* node, Node* right_sibling, int parent_key_pos) {
        Node* parent = node->parent;
        std::unique_lock<std::shared_mutex> node_lock(node->mutex);
        std::unique_lock<std::shared_mutex> right_lock(right_sibling->mutex);

        if (node->is_leaf) {
            // Merge leaf nodes
            node->keys.insert(node->keys.end(), right_sibling->keys.begin(), right_sibling->keys.end());
            node->values.insert(node->values.end(), right_sibling->values.begin(), right_sibling->values.end());
            node->next = right_sibling->next;
        } else {
            // Merge internal nodes
            node->keys.push_back(parent->keys[parent_key_pos]);
            node->keys.insert(node->keys.end(), right_sibling->keys.begin(), right_sibling->keys.end());
            node->children.insert(node->children.end(), right_sibling->children.begin(), right_sibling->children.end());

            for (Node* child : right_sibling->children) {
                std::unique_lock<std::shared_mutex> child_lock(child->mutex);
                child->parent = node;
            }
        }

        parent->keys.erase(parent->keys.begin() + parent_key_pos);
        parent->children.erase(parent->children.begin() + parent_key_pos + 1);

        node_lock.unlock();
        right_lock.unlock();
        delete right_sibling;

        if (parent != root && parent->keys.size() < MIN_KEYS) {
            handle_underflow(parent);
        } else if (parent == root && parent->keys.empty()) {
            root = node;
            node->parent = nullptr;
            delete parent;
        }
    }

    void print_node(Node* node, int level) {
        if (!node) return;

        std::shared_lock<std::shared_mutex> node_lock(node->mutex);
        
        for (int i = 0; i < level; i++) {
            std::cout << "  ";
        }

        std::cout << (node->is_leaf ? "Leaf: " : "Internal: ");
        for (int i = 0; i < node->keys.size(); i++) {
            std::cout << node->keys[i];
            if (node->is_leaf) {
                std::cout << "(" << node->values[i] << ")";
            }
            if (i < node->keys.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;

        // Copy children vector to avoid holding lock during recursion
        std::vector<Node*> children_copy;
        if (!node->is_leaf) {
            children_copy = node->children;
        }
        
        node_lock.unlock();

        if (!node->is_leaf) {
            for (Node* child : children_copy) {
                print_node(child, level + 1);
            }
        }
    }

public:
    BPlusTree() {
        root = new Node(true);
    }

    ~BPlusTree() {
        std::unique_lock<std::shared_mutex> tree_lock(tree_mutex);
        delete root;
    }

    void insert(const K& key, const V& value) {
        Node* leaf = find_leaf(key);
        insert_into_leaf(leaf, key, value);

        // Check if split is needed
        {
            std::shared_lock<std::shared_mutex> leaf_lock(leaf->mutex);
            if (leaf->keys.size() > MAX_KEYS) {
                leaf_lock.unlock();
                split_leaf(leaf);
            }
        }
    }

    bool search(const K& key, V& value) {
        Node* leaf = find_leaf(key);
        std::shared_lock<std::shared_mutex> leaf_lock(leaf->mutex);
        
        auto it = std::find(leaf->keys.begin(), leaf->keys.end(), key);
        if (it != leaf->keys.end()) {
            int pos = it - leaf->keys.begin();
            value = leaf->values[pos];
            return true;
        }
        return false;
    }

    bool remove(const K& key) {
        Node* leaf = find_leaf(key);
        return delete_from_leaf(leaf, key);
    }

    void print() {
        std::shared_lock<std::shared_mutex> tree_lock(tree_mutex);
        std::cout << "B+ Tree Structure:" << std::endl;
        print_node(root, 0);
        std::cout << std::endl;
    }

    void print_leaf_sequence() {
        std::shared_lock<std::shared_mutex> tree_lock(tree_mutex);
        std::cout << "Leaf sequence: ";
        
        Node* leaf = root;
        while (!leaf->is_leaf) {
            std::shared_lock<std::shared_mutex> node_lock(leaf->mutex);
            Node* next_leaf = leaf->children[0];
            node_lock.unlock();
            leaf = next_leaf;
        }

        while (leaf) {
            std::shared_lock<std::shared_mutex> leaf_lock(leaf->mutex);
            for (int i = 0; i < leaf->keys.size(); i++) {
                std::cout << leaf->keys[i] << "(" << leaf->values[i] << ") ";
            }
            Node* next_leaf = leaf->next;
            leaf_lock.unlock();
            leaf = next_leaf;
        }
        std::cout << std::endl;
    }
};

// Thread-safe example usage
// #include <thread>
// #include <random>
// #include <chrono>

// int main() {
//     std::cout << "=== Thread-Safe B+ Tree Example ===" << std::endl;
//     BPlusTree<int, std::string> tree;

//     // Function to insert random values
//     auto insert_worker = [&tree](int thread_id, int start, int end) {
//         std::random_device rd;
//         std::mt19937 gen(rd());
//         std::uniform_int_distribution<> dis(start, end);
        
//         for (int i = 0; i < 10; ++i) {
//             int key = dis(gen);
//             std::string value = "Thread" + std::to_string(thread_id) + "_Value" + std::to_string(key);
//             tree.insert(key, value);
//             std::this_thread::sleep_for(std::chrono::milliseconds(10));
//         }
//     };

//     // Function to search for values
//     auto search_worker = [&tree](int thread_id) {
//         std::random_device rd;
//         std::mt19937 gen(rd());
//         std::uniform_int_distribution<> dis(1, 100);
        
//         for (int i = 0; i < 10; ++i) {
//             int key = dis(gen);
//             std::string value;
//             if (tree.search(key, value)) {
//                 std::cout << "Thread " << thread_id << " found key " << key << " with value: " << value << std::endl;
//             }
//             std::this_thread::sleep_for(std::chrono::milliseconds(15));
//         }
//     };

//     // Create multiple threads
//     std::vector<std::thread> threads;
    
//     // Insert threads
//     for (int i = 0; i < 3; ++i) {
//         threads.emplace_back(insert_worker, i, i * 20 + 1, (i + 1) * 20);
//     }
    
//     // Search threads
//     for (int i = 0; i < 2; ++i) {
//         threads.emplace_back(search_worker, i + 10);
//     }
    
//     // Wait for all threads to complete
//     for (auto& t : threads) {
//         t.join();
//     }
    
//     std::cout << "\nFinal tree structure:" << std::endl;
//     tree.print();
//     tree.print_leaf_sequence();

//     return 0;
// }
#endif