#include "b_plus_tree.h"

int BPTree::remove(keys_struct x) {
  // delete logic
  if (root == nullptr) 
  {
    throw std::logic_error("Tree is empty!");
  } 
  else {
    Node *cursor = root;
    Node *parent;

    int leftSibling, rightSibling;
    // in the following while loop, cursor will will travel to the leaf node
    // possibly consisting the key
    while (!cursor->isLeaf) {
        parent = cursor;
        // Same logic as insert, search -> TODO refactor?
        if (x.key_value >= cursor->key[cursor->size -1].key_value)
        {
            i = cursor->size;
        }
        else {
            int i = 0;
            while (x.key_value >= cursor->key[i].key_value ) 
            {
                i++;
            }
            // i is now the pointer index
        }
        leftSibling = i - 1;
        rightSibling = i + 1;
        cursor = cursor->ptr[i];
    }
    // Search for the key if it exists.
    bool keyExists = false;
    int pos;
    for (pos = 0; pos < cursor->size; pos++) {
      if (cursor->key[pos].key_value == x.key_value) {
        keyExists = true;
        break;
      }
    }
    if (!keyExists) // if key does not exist in that leaf node
    {
      // cout<<"Not found\n";
      return;
    }

    //Delete the key by shifting all elements that are above the key down.
    for (int i = pos; i < cursor->size; i++) {
      cursor->key[i] = cursor->key[i + 1];
    }
    cursor->size--;

    if (cursor == root) // if it is root node, then make all pointers NULL
    {
      cout << "Deleted " << x.key_value << " "
           << "from leaf node successfully\n";
      for (int i = 0; i < MAX + 1; i++) {
        cursor->ptr[i] = NULL;
      }
      if (cursor->size == 0) // if all keys are deleted
      {
        // cout<<"Tree died\n";
        delete[] cursor->key;
        delete[] cursor->ptr;
        delete cursor;
        cout << "Deleted 1"
             << "\n";
        root = NULL;
      }
      return;
    }

    cursor->ptr[cursor->size] = cursor->ptr[cursor->size + 1];
    cursor->ptr[cursor->size + 1] = NULL;
    cout << "Deleted " << x.key_value << " "
         << " from leaf node successfully\n";

    // There are sufficient keys in node.
    if (cursor->size >= (MAX + 1) / 2)
    {
      return;
    }
    // Underflow condition. 
    // Case 1: Try to transfer a key from sibling node.
    // Check if left sibling exists.
    if (leftSibling >= 0) {
      Node *leftNode = parent->ptr[leftSibling];
      // Check if it is possible to transfer.
      if (leftNode->size >= (MAX + 1) / 2 + 1) {
        // Create space for transferring key.
        for (int i = cursor->size; i > 0; i--) {
          cursor->key[i] = cursor->key[i - 1];
        }
        // Update pointer to the next node.
        cursor->size++;
        cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
        cursor->ptr[cursor->size - 1] = nullptr;
        
        // Transfer key from left sibling to node.
        cursor->key[0] = leftNode->key[leftNode->size - 1];

        // Shift pointer from left sibling.
        leftNode->size--;
        leftNode->ptr[leftNode->size] = cursor;
        leftNode->ptr[leftNode->size + 1] = nullptr;

        // Update parent.
        parent->key[leftSibling] = cursor->key[0];
        return;
      }
    }
    // Check if right sibling exists.
    if (rightSibling <= parent->size)
    {
      Node *rightNode = parent->ptr[rightSibling];
      // Check if it is possible to transfer.
      if (rightNode->size >= (MAX + 1) / 2 + 1) {

        // shift pointer to next leaf
        cursor->size++;
        cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
        cursor->ptr[cursor->size - 1] = NULL;
        // transfer
        cursor->key[cursor->size - 1] = rightNode->key[0];

        // Update pointer of right sibling.
        rightNode->size--;
        rightNode->ptr[rightNode->size] = rightNode->ptr[rightNode->size + 1];
        rightNode->ptr[rightNode->size + 1] = NULL;
        
        // Shift keys of right sibling down by 1.
        for (int i = 0; i < rightNode->size; i++) {
          rightNode->key[i] = rightNode->key[i + 1];
        }
        // update parent
        parent->key[rightSibling - 1] = rightNode->key[0];
        return;
      }
    }
    // must merge and delete a node
    if (leftSibling >= 0) // if left sibling exist
    {
      Node *leftNode = parent->ptr[leftSibling];
      // transfer all keys to leftsibling and then transfer pointer to next leaf
      // node
      for (int i = leftNode->size, j = 0; j < cursor->size; i++, j++) {
        leftNode->key[i] = cursor->key[j];
      }
      leftNode->ptr[leftNode->size] = NULL;
      leftNode->size += cursor->size;
      leftNode->ptr[leftNode->size] = cursor->ptr[cursor->size];

      removeInternal(parent->key[leftSibling], parent, cursor); // delete parent node key
      delete[] cursor->key;
      delete[] cursor->ptr;
      delete cursor;
      cout << "Deleted 1"
           << "\n";
    } else if (rightSibling <= parent->size) // if right sibling exist
    {
      Node *rightNode = parent->ptr[rightSibling];
      // transfer all keys to cursor and then transfer pointer to next leaf node
      for (int i = cursor->size, j = 0; j < rightNode->size; i++, j++) {
        cursor->key[i] = rightNode->key[j];
      }
      cursor->ptr[cursor->size] = NULL;
      cursor->size += rightNode->size;
      cursor->ptr[cursor->size] = rightNode->ptr[rightNode->size];
      // cout<<"Merging two leaf nodes\n";
      removeInternal(parent->key[rightSibling - 1], parent,
                     rightNode); // delete parent node key
      delete[] rightNode->key;
      delete[] rightNode->ptr;
      delete rightNode;
      cout << "Deleted 1 \n";
    }
  }
}
