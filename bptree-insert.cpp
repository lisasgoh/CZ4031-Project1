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
      // // Iterate through the keys to find the relevant key/ptr
      // for (int i = 0; i < cursor->size; i++) {

      //     // If the key is smaller than the current key, go to the pointer's node.
      //   if (x.key_value < cursor->key[i].key_value) {
      //     cursor = cursor->ptr[i];
      //     break;
      //   }

      //   // If the key is larger than all keys in the node, go to the last pointer's node.
      //   if (i == cursor->size - 1) {
      //     cursor = cursor->ptr[i + 1];
      //     break;
      //   }
      // }
        int i = 0;
        while (x.key_value >= cursor->key[i++]);
        parent = cursor;
        cursor = cursor->ptr[i];
    }
    // At this point, cursor is the leaf node in which the new key will be inserted.
    if (cursor->size < MAX) {
      int i = 0;

      // Continue iterating while cursor has not reached the last key and the key to be inserted is larger
      // than the current key.
      while (x.key_value > cursor->key[i].key_value && i < cursor->size)
      {
        i++;
      }

      if (cursor->key[i].key_value == x.key_value)
      {
         cursor->key[i].add_vect.push_back(x.add_vect[0]);
      }
      else {
      // make space for new key
        for (int j = cursor->size; j > i; j--) 
        {
            cursor->key[j] = cursor->key[j - 1];
            // TODO: shift pointesr??
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
      // cout<<"Inserted "<< x.key_value << " " << x.add_vect[0] <<"
      // successfully\n"; cout<<"Overflow in leaf node!\nSplitting leaf node\n";
      // overflow condition
      // create new leaf node
      Node *newLeaf = new Node;
      // Create a virtual node to accommodate all the keys and insert x into it
      keys_struct virtualNode[MAX + 1];
      for (int i = 0; i < MAX; i++) {
        virtualNode[i] = cursor->key[i];
        // Copy pointers????
      }

      // Find the point to insert the new key.
      int i = 0, j;
      while (x.key_value > virtualNode[i].key_value && i < MAX) {
        i++;
      }

      // Key is already present
      if (i < cursor->size && cursor->key[i] == x.key_value) {
         cursor->key[i].add_vect.push_back(x.add_vect[0]);
         return;
      }

      // Create space for new key in virtual node.
      //TODO: should be max???
      for (int j = MAX + 1; j > i; j--) {
        virtualNode[j] = virtualNode[j - 1];
      }

      // Insert the new key.
      virtualNode[i] = x;

      newLeaf->isLeaf = true;

      // Split the cursor into two leaf nodes. 
      // ⌊(n+1)/2⌋ keys for left, n+1-⌊(n+1)/2⌋ keys for right.
      cursor->size = (MAX + 1) / 2;
      newLeaf->size = MAX + 1 - (MAX + 1) / 2;


      // Set the cursor to point to the new leaf node.
      cursor->ptr[cursor->size] = newLeaf;
      // make new leaf node point to the next leaf node
      newLeaf->ptr[newLeaf->size] = cursor->ptr[MAX];
      cursor->ptr[MAX] = NULL;
      // now give elements to new leaf nodes
      for (i = 0; i < cursor->size; i++) {
        cursor->key[i] = virtualNode[i];
      }

    //   for (i = 0, j = cursor->size; i < newLeaf->size; i++, j++) {
    //     newLeaf->key[i] = virtualNode[j];
    //   }
      for (int j = 0; j < newLeaf->size; i++, j++) {
          newLeaf->key[j] = virtualNode[i];
      }

      // modify the parent
      if (cursor == root) {
        // if cursor is a root node, we create a new root
        Node *newRoot = new Node;

         //Set the new root's key to be the left bound of the right child.
        newRoot->key[0] = newLeaf->key[0];
        newRoot->ptr[0] = cursor;
        newRoot->ptr[1] = newLeaf;

        // Update attributes of new root
        newRoot->isLeaf = false;
        newRoot->size = 1;
        root = newRoot;
        // cout<<"Created new root\n";
      } else {
        // insert new key in parent node
        insertInternal(newLeaf->key[0], parent, newLeaf);
      }
    }
  }
}