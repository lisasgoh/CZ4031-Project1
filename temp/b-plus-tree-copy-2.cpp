// int MAX; //size of each node
#include "b_plus_tree.h"
using namespace std;
extern void *startAddress;

Node::Node() {
  // dynamic memory allocation
  key = new keys_struct[MAX];
  ptr = new Node *[MAX + 1];
  IS_LEAF = true;
}

BPTree::BPTree() : root{NULL} {}