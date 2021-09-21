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
  bool IS_LEAF;
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
  void removeInternal(keys_struct x, Node *cursor, Node *child);
  Node *findParent(Node *cursor, Node *child);

public:
  BPTree();
  Node *search(float x, bool flag, bool printer);
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