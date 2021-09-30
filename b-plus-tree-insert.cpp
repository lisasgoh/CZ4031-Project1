#include "b-plus-tree.h";


void BPTree::insert(keys_struct x) {
  // insert logic
  if (root == nullptr) 
  {
    root = new Node;
    root->key[0] = x;
    root->isLeaf = true;
    root->size = 1;
    // cout<<"Created root\nInserted "<<  x.key_value << " " << x.add_vect[0]
    // <<" successfully\n";
  } else {

    Node *cursor = root;
    Node *parent;

    // While not leaf, the cursor will continue travelling down to the leaf node
    // possibly containing the key.
    while (!cursor->isLeaf) {
        if (x.key_value >= cursor->key[cursor->size -1].key_value)
        {
            i = cursor->size;
        }
        else
        {
             // // Iterate through the keys to find the relevant key/ptr
            int i = 0;
            while (x.key_value >= cursor->key[i++]);
            parent = cursor;
            cursor = cursor->ptr[i];
        }
    }
    // Cursor is now the leaf node in which the new key will be inserted.
    // If there is space to insert the new key.
    if (cursor->size < MAX) {
      int i = 0;

      // Continue iterating while cursor has not reached the last key and the key to be inserted is larger
      // than the current key.
      while (x.key_value > cursor->key[i].key_value && i < cursor->size) i++;

      if (cursor->key[i].key_value == x.key_value)
      {
        // Insert into LL
         cursor->key[i].add_vect.push_back(x.add_vect[0]);
      }
      else {
        // Make space for new key
        for (int j = cursor->size; j > i; j--) 
        {
            cursor->key[j] = cursor->key[j - 1];
            // TODO: shift pointesr?? (ptrs are null here??)
        }

        // Insert key into the node.
        cursor->key[i] = x;
        cursor->size++;
        cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
        cursor->ptr[cursor->size - 1] = NULL;
        // cout<<"Inserted "<< x.key_value << " " << x.add_vect[0] <<"
        // successfully\n";
      }
      //Overflow, due to insufficient space to insert new key.
    } else {
      Node *newLeaf = new Node;
      newLeaf->isLeaf = true;

      // Create a temp node to accommodate all the keys and insert x into it
      keys_struct tempNode[MAX + 1];
      for (int i = 0; i < MAX; i++) { //cursor-size == MAX
        tempNode[i] = cursor->key[i];
        // Copy pointers????
      }

      // Find the point to insert the new key.
      if (x.key_value > tempNode[MAX - 1]) 
      {
        tempNode[MAX] = x;
      }
      else 
      {
        int i = 0;
        while (x.key_value > tempNode[i].key_value) i++;

        // Key is already present
        if (cursor->key[i] == x.key_value) {
          cursor->key[i].add_vect.push_back(x.add_vect[0]);
          return;
        }

        // Create space for new key in virtual node.
        //TODO: should be max???
        for (int j = MAX + 1; j > i; j--) {
          tempNode[j] = tempNode[j - 1];
        }

        // Insert the new key.
        tempNode[i] = x;
      }


      // Split the cursor into two leaf nodes. 
      // ⌊(n+1)/2⌋ keys for left, n+1-⌊(n+1)/2⌋ keys for right.
      cursor->size = (MAX + 1) / 2;
      newLeaf->size = MAX + 1 - (MAX + 1) / 2;

      // Set the cursor to point to the new leaf node.
      cursor->ptr[cursor->size] = newLeaf;

      // Set the rightmost pointer of the new leaf node to the next leaf node.
      newLeaf->ptr[newLeaf->size] = cursor->ptr[MAX];
      cursor->ptr[MAX] = NULL;

      // Distribute elements to the two new leaf nodes.
      for (i = 0; i < cursor->size; i++) {
        cursor->key[i] = tempNode[i];
      }

      for (int j = 0; j < newLeaf->size; i++, j++) {
          newLeaf->key[j] = tempNode[i];
      }

      // Modify the parent node.
      if (cursor == root) {
        // if cursor is a root node, we create a new root.
        Node *newRoot = new Node;

        //Set the new root's key to be the left bound of the right child.
        newRoot->key[0] = newLeaf->key[0];
        newRoot->ptr[0] = cursor;
        newRoot->ptr[1] = newLeaf;

        // Update attributes of new root.
        newRoot->isLeaf = false;
        newRoot->size = 1;
        root = newRoot;
      } else {
        // Insert new key in parent node.
        insertInternal(newLeaf->key[0], parent, newLeaf);
      }
    }
  }
}