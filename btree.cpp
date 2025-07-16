#include<iostream>
using namespace std;



class BtreeNode{
    int *keys; // array of keys
    int t; 
    BtreeNode **C;
    int n; // current no of keys
    bool leaf;

    public:
        BtreeNode(int _t,bool _leaf);
        void insertNotFull(int k);
        void splitChild(int i,BtreeNode *y);
        void traverse();
        BtreeNode * search(int k);

    friend class BTree;    
};  


class BTree{
    BtreeNode * root;
    int t; // minimum degreee

    public:
    BTree(int _t){
        root = NULL;
        t = _t;

    }

    void traverse(){
        if(root!=NULL){
            root->traverse();
        }
    }

    BtreeNode* search(int k)
    {  return (root == NULL)? NULL : root->search(k); }

    
    void insert(int k);
};

BtreeNode::BtreeNode(int t1, bool leaf1)
{
    // Copy the given minimum degree and leaf property
    t = t1;
    leaf = leaf1;

    // Allocate memory for maximum number of possible keys
    // and child pointers
    keys = new int[2*t-1];
    C = new BtreeNode *[2*t];

    // Initialize the number of keys as 0
    n = 0;
}

void BtreeNode::traverse(){
    int i;
    for(i=0;i<n;i++){
        if(leaf==false){
            C[i]->traverse();
        }
        cout<<" "<<keys[i];
    }

    // Print the subtree rooted with last child
    if(leaf==false){
        C[i]->traverse();
    }
}

BtreeNode *BtreeNode::search(int k){
    int i = 0;
    while(i<n && k > keys[i]){
        i++;
    }

    if(keys[i]==k){
        return this;
    }

    if(leaf==true){
        return NULL;
    }

    return C[i]->search(k);
}
/*
1,2,3,4,

  2
1   3,4  
*/

void BTree::insert(int k){
    if(root==NULL){
        root = new BtreeNode(t,true);
        root->keys[0] = k;
        root->n = 1;
    }else{
        if(root->n==2*t-1){
            BtreeNode *s = new BtreeNode(t,false);
            s->C[0] = root;
            s->splitChild(0,root);
            int i = 0;
            if(s->keys[0]<k){

                i++;
            }
            s->C[i]->insertNotFull(k);
            root = s;
        }else{
            root->insertNotFull(k);
        }
    }
}

void BtreeNode::insertNotFull(int k){
    int i = n-1;
    if(leaf==true){
        while(i>=0 && keys[i]>k){
            keys[i+1] = keys[i];
            i--;
        }
        keys[i+1] = k;
        n = n+1;
    }

    else{
        while(i>=0 && keys[i]>k){
            i--;
        }

        if(C[i+1]->n==2*t-1){
            splitChild(i+1,C[i+1]);
            if(keys[i+1]<k){
                i++;
            }
        }

        C[i+1]->insertNotFull(k);
    }
}


void BtreeNode::splitChild(int i,BtreeNode *y){
    BtreeNode * z = new BtreeNode(y->t,y->leaf);
    z->n = t-1;
    for(int j=0;j<t-1;j++){
        z->keys[j] = y->keys[j+t];
    }
    if (y->leaf == false)
    {
        for (int j = 0; j < t; j++)
            z->C[j] = y->C[j+t];
    }
    y->n = t-1;

    for(int j=n;j>=i+1;j--){
        C[j+1] = C[j];
    }
    C[i+1] = z;

    for(int j=n-1;j>=i;j--){
        keys[j+1] = keys[j];
    }
    keys[i] = y->keys[t-1];
    n = n+1;


}

int main(int argc, char const *argv[])
{
     BTree t(3); // A B-Tree with minimum degree 3
    t.insert(10);
    t.insert(20);
    t.insert(5);
    t.insert(6);
    t.insert(12);
    t.insert(30);
    t.insert(7);
    t.insert(17);

    cout << "Traversal of the constructed tree is ";
    t.traverse();

    return 0;
}
