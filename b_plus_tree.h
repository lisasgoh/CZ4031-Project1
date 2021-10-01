#ifndef B_PLUS_TREE_H
#define B_PLUS_TREE_H

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
class Node {
  bool isLeaf;
  keys_struct *key;
  int size;
  Node **ptr;
  friend class BPTree;

public:
  Node();
};

class BPTree {
  Node *root;
  void insertInternal(keys_struct x, Node *cursor, Node *child);
  int removeInternal(keys_struct x, Node *cursor, Node *child, Node *newNode);
  Node *findParent(Node *cursor, Node *child);
  int numNode = 0;

public:
  BPTree();
  void searchSingle(float key);
  void searchRange(float lowerKeyBound, float upperKeyBound);
  Node * search2(float x, bool flag, bool printer);
  int height(Node *cursor);
  void insert(keys_struct x);
  int remove(keys_struct x);
  int display(Node *cursor, int nodecount, bool first);
  Node *getRoot();
  int getMax();
  int getNumNode() { return numNode; }
  void printTree(Node *cursor);
  //    void cleanUp(Node* cursor)
  //    {
  //        //clean up logic
  //        if(cursor!=NULL)
  //        {
  //            if(cursor->IS_LEAF != true)
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

#endif
