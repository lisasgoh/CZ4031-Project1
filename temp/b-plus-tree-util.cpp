#include "b_plus_tree.h"

Node *BPTree::findParent(Node *cursor, Node *child) {
  // find parent of child node within cursor use DFS
  // returns NULL if child has no parent in cursor
  Node *parent;
  // skip search if cursor is on last/second last level
  // cursor cannot be a parent if it is leaf node
  // cursor cannot be second last level since function is not called when child
  // node is leaf node during insertion
  if (cursor->IS_LEAF || (cursor->ptr[0])->IS_LEAF) {
    return NULL;
  }
  // go through each pointer of cursor
  for (int i = 0; i < cursor->size + 1; i++) {
    // if pointer points to child, cursor is the parent
    if (cursor->ptr[i] == child) {
      parent = cursor;
      return parent;
    }
    // else try to find parent among children of cursor
    else {
      parent = findParent(cursor->ptr[i], child);
      if (parent != NULL)
        return parent;
    }
  }
  return parent;
}


// recursively traverse tree to find height
int BPTree::height(Node *cursor) {
  if (cursor->IS_LEAF == false) {
    // for each node traversed before leaf, height + 1
    return height(cursor->ptr[0]) + 1;
  } else if (cursor->IS_LEAF == true) {
    // reached leaf
    return 1;
  } else {
    return NULL;
  }
}

int BPTree::getMax() { return MAX; }

Node *BPTree::getRoot() { return root; }
