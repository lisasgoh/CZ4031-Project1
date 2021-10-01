#include "memory-pool.h"
#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

struct keys_struct {
  float key_value;
  vector<void *> add_vect;
};

class BPTree; // self explanatory classes
class Node 
{
private:
  bool isLeaf;
  // float *keys; // Array of keys
  keys_struct *key; //Array of keys with the linkedlist thing
  int size;
  Node **ptr;
  friend class BPTree;

public:
  Node();
  Node(bool isLeaf) {this.isLeaf = isLeaf;};
  // Node(bool isLeaf) {this.isLeaf = isLeaf;};
};

class BPTree {
  Node *root;
  void insertInternal(keys_struct x, Node *cursor, Node *child);
  void removeInternal(keys_struct x, Node *cursor, Node *child);
  Node *findParent(Node *cursor, Node *child);

public:
  BPTree();
  Node *search(float lowerBoundKey, float upperBoundKey);
  int height(Node *cursor);
  void insert(keys_struct x);
  void remove(keys_struct x);
  int display(Node *cursor, int nodecount, bool first);
  Node *getRoot();
  //    void cleanUp(Node* cursor)
  //    {
  //        //clean up logic
  //        if(cursor!=NULL)
  //        {
  //            if(cursor->isLeaf != true)
  //            {
  //                for(int i = 0; i < cursor->size+1; i++)
  //                {
  //                    cleanUp(cursor->ptr[i]);
  //                }
  //            }
  //            for(int i = 0; i < cursor->size; i++)
  //            {
  //                cout<<"Deleted key from memory: "<<cursor->key[i].key_value
  //                << " " << cursor->key[i].add_vect[0] <<"\n";
  //            }
  //            delete[] cursor->key;
  //            delete[] cursor->ptr;
  //            delete cursor;
  //        }
  //    }
  //    ~BPTree()
  //    {
  //        //calling cleanUp routine
  //        cleanUp(root);
  //    }
};
