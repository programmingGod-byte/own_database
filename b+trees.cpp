#include <iostream>
#include <vector>
#include <algorithm>

class BPlusTree {
private:
    static const int DEGREE = 3;
    static const int MAX_KEYS = 2 * DEGREE - 1;
    static const int MIN_KEYS = DEGREE - 1;

    struct Node {
        std::vector<int> keys;
        std::vector<int> values;  
        
        std::vector<Node*> children;  
        bool is_leaf;
        Node* next;  
        Node* parent;

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

    Node* find_leaf(int key) {
        Node* node = root;
        while (!node->is_leaf) {
            int i = 0;
            while (i < node->keys.size() && key >= node->keys[i]) {
                i++;
            }
            node = node->children[i];
        }
        return node;
    }

    void insert_into_leaf(Node* leaf, int key, int value) {
        auto it = std::lower_bound(leaf->keys.begin(), leaf->keys.end(), key);
        int pos = it - leaf->keys.begin();

        if (it != leaf->keys.end() && *it == key) {

            leaf->values[pos] = value;
        } else {

            leaf->keys.insert(it, key);
            leaf->values.insert(leaf->values.begin() + pos, value);
        }
    }

    void split_leaf(Node* leaf) {
        Node* new_leaf = new Node(true);
        int mid = (leaf->keys.size() + 1) / 2;

        new_leaf->keys.assign(leaf->keys.begin() + mid, leaf->keys.end());
        new_leaf->values.assign(leaf->values.begin() + mid, leaf->values.end());

        leaf->keys.resize(mid);
        leaf->values.resize(mid);

        new_leaf->next = leaf->next;
        leaf->next = new_leaf;

        new_leaf->parent = leaf->parent;

        int promote_key = new_leaf->keys[0];
        insert_into_parent(leaf, promote_key, new_leaf);
    }

    void insert_into_parent(Node* left, int key, Node* right) {
        if (left == root) {

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

        auto it = std::lower_bound(parent->keys.begin(), parent->keys.end(), key);
        int pos = it - parent->keys.begin();

        parent->keys.insert(it, key);
        parent->children.insert(parent->children.begin() + pos + 1, right);
        right->parent = parent;

        if (parent->keys.size() > MAX_KEYS) {
            split_internal(parent);
        }
    }

    void split_internal(Node* node) {
        Node* new_node = new Node(false);
        int mid = node->keys.size() / 2;
        int promote_key = node->keys[mid];

        new_node->keys.assign(node->keys.begin() + mid + 1, node->keys.end());
        new_node->children.assign(node->children.begin() + mid + 1, node->children.end());

        for (Node* child : new_node->children) {
            child->parent = new_node;
        }

        node->keys.resize(mid);
        node->children.resize(mid + 1);

        new_node->parent = node->parent;

        insert_into_parent(node, promote_key, new_node);
    }

    bool delete_from_leaf(Node* leaf, int key) {
        auto it = std::find(leaf->keys.begin(), leaf->keys.end(), key);
        if (it == leaf->keys.end()) {
            return false;  
        }

        int pos = it - leaf->keys.begin();
        leaf->keys.erase(it);
        leaf->values.erase(leaf->values.begin() + pos);

        if (leaf != root && leaf->keys.size() < MIN_KEYS) {
            handle_underflow(leaf);
        }

        return true;
    }

    void handle_underflow(Node* node) {
        Node* parent = node->parent;
        if (!parent) return;  

        int pos = 0;
        while (pos < parent->children.size() && parent->children[pos] != node) {
            pos++;
        }

        if (pos > 0) {
            Node* left_sibling = parent->children[pos - 1];
            if (left_sibling->keys.size() > MIN_KEYS) {
                borrow_from_left(node, left_sibling, pos - 1);
                return;
            }
        }

        if (pos < parent->children.size() - 1) {
            Node* right_sibling = parent->children[pos + 1];
            if (right_sibling->keys.size() > MIN_KEYS) {
                borrow_from_right(node, right_sibling, pos);
                return;
            }
        }

        if (pos > 0) {
            merge_with_left(node, parent->children[pos - 1], pos - 1);
        } else {
            merge_with_right(node, parent->children[pos + 1], pos);
        }
    }

    void borrow_from_left(Node* node, Node* left_sibling, int parent_key_pos) {
        Node* parent = node->parent;

        if (node->is_leaf) {

            int borrowed_key = left_sibling->keys.back();
            int borrowed_value = left_sibling->values.back();

            left_sibling->keys.pop_back();
            left_sibling->values.pop_back();

            node->keys.insert(node->keys.begin(), borrowed_key);
            node->values.insert(node->values.begin(), borrowed_value);

            parent->keys[parent_key_pos] = borrowed_key;
        } else {

            int parent_key = parent->keys[parent_key_pos];
            int borrowed_key = left_sibling->keys.back();
            Node* borrowed_child = left_sibling->children.back();

            left_sibling->keys.pop_back();
            left_sibling->children.pop_back();

            node->keys.insert(node->keys.begin(), parent_key);
            node->children.insert(node->children.begin(), borrowed_child);
            borrowed_child->parent = node;

            parent->keys[parent_key_pos] = borrowed_key;
        }
    }

    void borrow_from_right(Node* node, Node* right_sibling, int parent_key_pos) {
        Node* parent = node->parent;

        if (node->is_leaf) {

            int borrowed_key = right_sibling->keys.front();
            int borrowed_value = right_sibling->values.front();

            right_sibling->keys.erase(right_sibling->keys.begin());
            right_sibling->values.erase(right_sibling->values.begin());

            node->keys.push_back(borrowed_key);
            node->values.push_back(borrowed_value);

            parent->keys[parent_key_pos] = right_sibling->keys.front();
        } else {

            int parent_key = parent->keys[parent_key_pos];
            int borrowed_key = right_sibling->keys.front();
            Node* borrowed_child = right_sibling->children.front();

            right_sibling->keys.erase(right_sibling->keys.begin());
            right_sibling->children.erase(right_sibling->children.begin());

            node->keys.push_back(parent_key);
            node->children.push_back(borrowed_child);
            borrowed_child->parent = node;

            parent->keys[parent_key_pos] = borrowed_key;
        }
    }

    void merge_with_left(Node* node, Node* left_sibling, int parent_key_pos) {
        Node* parent = node->parent;

        if (node->is_leaf) {
            left_sibling->keys.insert(left_sibling->keys.end(), node->keys.begin(), node->keys.end());
            left_sibling->values.insert(left_sibling->values.end(), node->values.begin(), node->values.end());
            left_sibling->next = node->next;
        } else {

            left_sibling->keys.push_back(parent->keys[parent_key_pos]);
            left_sibling->keys.insert(left_sibling->keys.end(), node->keys.begin(), node->keys.end());
            left_sibling->children.insert(left_sibling->children.end(), node->children.begin(), node->children.end());

            for (Node* child : node->children) {
                child->parent = left_sibling;
            }
        }

        parent->keys.erase(parent->keys.begin() + parent_key_pos);
        parent->children.erase(parent->children.begin() + parent_key_pos + 1);

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

        if (node->is_leaf) {

            node->keys.insert(node->keys.end(), right_sibling->keys.begin(), right_sibling->keys.end());
            node->values.insert(node->values.end(), right_sibling->values.begin(), right_sibling->values.end());
            node->next = right_sibling->next;
        } else {

            node->keys.push_back(parent->keys[parent_key_pos]);
            node->keys.insert(node->keys.end(), right_sibling->keys.begin(), right_sibling->keys.end());
            node->children.insert(node->children.end(), right_sibling->children.begin(), right_sibling->children.end());

            for (Node* child : right_sibling->children) {
                child->parent = node;
            }
        }

        parent->keys.erase(parent->keys.begin() + parent_key_pos);
        parent->children.erase(parent->children.begin() + parent_key_pos + 1);

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

        if (!node->is_leaf) {
            for (Node* child : node->children) {
                print_node(child, level + 1);
            }
        }
    }

public:
    BPlusTree() {
        root = new Node(true);
    }

    ~BPlusTree() {
        delete root;
    }

    void insert(int key, int value) {
        Node* leaf = find_leaf(key);
        insert_into_leaf(leaf, key, value);

        if (leaf->keys.size() > MAX_KEYS) {
            split_leaf(leaf);
        }
    }

    bool search(int key, int& value) {
        Node* leaf = find_leaf(key);
        auto it = std::find(leaf->keys.begin(), leaf->keys.end(), key);
        if (it != leaf->keys.end()) {
            int pos = it - leaf->keys.begin();
            value = leaf->values[pos];
            return true;
        }
        return false;
    }

    bool remove(int key) {
        Node* leaf = find_leaf(key);
        return delete_from_leaf(leaf, key);
    }

    void print() {
        std::cout << "B+ Tree Structure:" << std::endl;
        print_node(root, 0);
        std::cout << std::endl;
    }

    void print_leaf_sequence() {
        std::cout << "Leaf sequence: ";
        Node* leaf = root;
        while (!leaf->is_leaf) {
            leaf = leaf->children[0];
        }

        while (leaf) {
            for (int i = 0; i < leaf->keys.size(); i++) {
                std::cout << leaf->keys[i] << "(" << leaf->values[i] << ") ";
            }
            leaf = leaf->next;
        }
        std::cout << std::endl;
    }
};

int main() {
    BPlusTree tree;

    tree.insert(10, 100);
    tree.insert(20, 200);
    tree.insert(5, 50);
    tree.insert(6, 60);
    tree.insert(12, 120);
    tree.insert(30, 300);
    tree.insert(7, 70);
    tree.insert(17, 170);

    std::cout << "After insertions:" << std::endl;
    tree.print();
    tree.print_leaf_sequence();

    int value;
    if (tree.search(12, value)) {
        std::cout << "Found key 12 with value: " << value << std::endl;
    }

    if (!tree.search(15, value)) {
        std::cout << "Key 15 not found" << std::endl;
    }

    tree.remove(6);
    tree.remove(12);

    std::cout << "\nAfter deletions:" << std::endl;
    tree.print();
    tree.print_leaf_sequence();

    return 0;
}