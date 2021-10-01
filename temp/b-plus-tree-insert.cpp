#include "b_plus_tree.h"

#include <vector>
#include <cstring>
#include <iostream>

using namespace std;

/**
 * @brief 
 * 
 * @param x The key of the new leaf node
 * @param cursor 
 * @param child 
 */
void BPTree::insertInternal(keys_struct x, Node* cursor, Node* child)
{
    // insert into current node if there is space in cursor
    if(cursor->size < MAX)
    {
        // since cursor is not full, linear search for position of new key
        int i = 0; // position of new key
        while (i < cursor->size && x.key_value > cursor->key[i].key_value) i++;

        //shift keys above new key up TODO refactor as a util?
        for(int j = cursor->size; j > i; j--)
        {
          cursor->key[j] = cursor->key[j-1];  //push each key above the position of inserted key up
          cursor->ptr[j+1] = cursor->ptr[j];
        }
        //shift pointers above new pointer up
        // for(int i = cursor->size+1; i > newKeyPos+1; i--)
        // {
        //     cursor->ptr[i] = cursor->ptr[i-1];
        // }
        cursor->key[i] = x;  // set new key
        cursor->size++; //set new size
        cursor->ptr[i+1] = child;   // set new pointer
    }
        // split internal node if overflow
    else
    {
        // create another internal node when overflow
        Node* newInternal = new Node;
        numNode++;
        newInternal->isLeaf = false;

        // Create temporary node to with extra key and pointer to store keys and pointers.
        keys_struct tempKeys[MAX+1];

         //MAX refers to the number of keys, pointer = number of keys + 1
        Node* tempPointers[MAX+2];

        // Copy keys and nodes to the temporary node.
        for (int i = 0; i < MAX; i++)
        {
            tempKeys[i] = cursor->key[i];
            tempPointers[i] = cursor->ptr[i];
        }
        tempPointers[MAX] = cursor->ptr[MAX];

        // for(int i = 0; i < MAX+1; i++)
        // {
        //     tempPointers[i] = cursor->ptr[i];
        // }
        int j;
        int i = 0;  // position of new key

        // since cursor is not full, linear search for position of new key
        while(i < MAX && x.key_value > tempKeys[i].key_value){
            i++;
        }

        //shift keys above new key up
        for(j = MAX+1; j > i; j--){
            tempKeys[j] = tempKeys[j-1];
            tempPointers[j+1] = tempPointers[j];
        }
        // Insert new key.
        tempKeys[i] = x; 

        // Insert new pointer.
        tempPointers[i+1] = child;

        //shift pointers above new pointer up
        // for(int j = MAX+2;j > i+1; j--){
        //     tempPointers[j] = tempPointers[j-1];
        // }

        // tempPointers[i+1] = child;  //set new pointer


        // split cursor to cursor and newInternal with half of original size
        cursor->size = (MAX+1)/2;
        newInternal->size = MAX-(MAX+1)/2;

        // set keys and pointers of new node
        for (i = 0, j = cursor->size+1; i < newInternal->size; i++, j++){
            newInternal->key[i] = tempKeys[j];
            newInternal->ptr[i] = tempPointers[j];
        }
        newInternal->ptr[newInternal->size] = tempPointers[j];
        // for(i = 0, j = cursor->size+1; i < newInternal->size+1; i++, j++){
        //     newInternal->ptr[i] = tempPointers[j];
        // }
        // the note we are splitting is a root node
        if (root == cursor)
        {
            //creation of new node when splitting root node
            Node* newRoot = new Node;
            numNode ++;
            newRoot->key[0] = cursor->key[cursor->size];
            newRoot->ptr[0] = cursor;
            newRoot->ptr[1] = newInternal;
            newRoot->isLeaf = false;
            newRoot->size = 1;
            root = newRoot;
            //cout<<"Created new root\n";
        }
        else
        {   // recursive DFS to find parent node
            insertInternal(cursor->key[cursor->size] ,findParent(root,cursor) ,newInternal);
        }
    }
}

/**
 * @brief 
 * 
 * @param x 
 * @param newKey 
 */
void BPTree::insert(keys_struct x, float newKey) {
  cout << root << endl;
  // insert logic
  if (root == nullptr) 
  {
    root = new Node;
    root->key[0] = x;
    root->isLeaf = true;
    root->size = 1;
    numNode++
    return;
  } 
  else 
  {
    Node *cursor = root;
    Node *parent;

    // While not leaf, the cursor will continue travelling down to the leaf node
    // possibly containing the key.
    while (!cursor->isLeaf) 
    {
        int i;
        if (x.key_value >= cursor->key[cursor->size -1].key_value)
        {
            i = cursor->size;
        }
        else
        {
             // // Iterate through the keys to find the relevant key/ptr
            i = 0;
            while (x.key_value >= cursor->key[i]) i++;
        }
        parent = cursor;
        cursor = cursor->ptr[i];
    }
    // Cursor is now the leaf node in which the new key will be inserted.
    // If there is space to insert the new key.
    if (cursor->size < MAX) {
      int i = 0;

      // Continue iterating while cursor has not reached the last key and the key to be inserted is larger
      // than the current key.
      while (i < cursor->size && x.key_value > cursor->key[i].key_value) i++;

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

      numNode++;

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
        if (cursor->key[i] == x.key_value) 
        {
          cursor->key[i].add_vect.push_back(x.add_vect[0]);
          return;
        }

        // Create space for new key in virtual node.
        //TODO: should be max???
        for (int j = MAX + 1; j > i; j--) 
        {
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
      for (i = 0; i < cursor->size; i++) 
      {
        cursor->key[i] = tempNode[i];
      }

      for (int j = 0; j < newLeaf->size; i++, j++) 
      {
        newLeaf->key[j] = tempNode[i];
      }

      // Modify the parent node.
      if (cursor == root) 
      {
        // if cursor is a root node, we create a new root.
        Node *newRoot = new Node;
          numNode++;

        //Set the new root's key to be the left bound of the right child.
        newRoot->key[0] = newLeaf->key[0];
        newRoot->ptr[0] = cursor;
        newRoot->ptr[1] = newLeaf;

        // Update attributes of new root.
        newRoot->isLeaf = false;
        newRoot->size = 1;
        root = newRoot;
      } 
      else 
      {
        // Insert new key in parent node.
        insertInternal(newLeaf->key[0], parent, newLeaf);
      }
    }
  }
}