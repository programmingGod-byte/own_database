#include<iostream>
using namespace std;


// order m ----> m-1 elements and m children
const int MAX = 10;
const int INT_MIN = -1000000;
struct node{
    int count;
    int value[MAX -1];
    struct node *child[MAX];
    bool isLeaf = true;
    node(){
        count = 0;
        for(int i=0;i<MAX;i++){
            child[i]  = nullptr;
            if(i!=MAX-1){
                value[i] = INT_MIN;
            }
        }
    }

    bool search(int val,struct node *n){
        struct node * temp = nullptr;
        if(n->value[0]>val) {
            temp = n->child[0];
        }
        if(val>n->value[MAX-2]){
            temp = n->child[MAX-2];
        }
        for(int i=0;i<MAX-1;i++){
            
                if(n->value[i]==val){
                    return true;
                }

                if(i!=MAX-2){
                    if(val>n->value[i]&& val<n->value[i+1]){
                        temp = n->child[i+1];
                    }
                }

                
            
        }

        if(n->isLeaf){
            return false;
        }

        bool ans = search(val,temp);


        return ans;
    }
    

};


int main(int argc, char const *argv[])
{
    
    return 0;
}
