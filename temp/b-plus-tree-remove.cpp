#include "b_plus_tree.h"

using namespace std;

void BPTree::removeInternal(keys_struct x, Node* cursor, Node* child)
{
    // If root node key is being deleted and root is empty after removal, child becomes new root.
    if(cursor == root && cursor->size == 1)
    {
        // check if larger or smaller pointer points to child
        // other pointer that do not point to child becomes new root
        int newRoot;
        // Check if the adjacent pointers points to child.
        if (cursor->ptr[0] == child || cursor->ptr[1] == child) //TODO is this check necessary
        {
            // // check if larger pointer points to child
            // if(cursor->ptr[1] == child){
            //     // set new root to other child
            //     root = cursor->ptr[0];
            // }
            // else {
            //     // set new root to other child
            //    root = cursor->ptr[1];
            // }
            root = cursor->ptr[0] == child ? cursor->ptr[1]: cursor->ptr[0]; 

            // Delete child node.
            delete[] child->key;
            delete[] child->ptr;
            delete child;
            numNode--;

            cout << "Deleted 1" <<"\n";
            // set new root
            // root = cursor->ptr[newRoot]

            // Delete old root node.
            delete[] cursor->key;
            delete[] cursor->ptr;
            delete cursor;
            // numNode --; ???????
            cout << "Deleted 1" <<"\n";
            //cout<<"Changed root node\n";
            return;
        }
    }
    // when reach here, parent is not root node with 1 key
    // // search for position of key to be deleted
    // for(pos = 0; pos < cursor->size; pos++){
    //     // position acquired
    //     if(cursor->key[pos].key_value == x.key_value){
    //         break;
    //     }
    // }

    // Search for the key that is to be deleted.
    int pos = 0;
    while (x.key_value != cursor->key[pos].key_value && pos < cursor->size) pos++;

    // shift each key from pos above back, removing the key
    for(int i = pos; i < cursor->size; i++){
        cursor->key[i] = cursor->key[i+1];
    }
    // search for position of pointer to be deleted
    // for(pos = 0; pos < cursor->size+1; pos++){
    //     // position acquired
    //     if(cursor->ptr[pos] == child){
    //         break;
    //     }
    // }
    pos = 0;
    while (cursor->ptr[pos] != child && pos < cursor->size + 1) pos++; //is this necessary lol

    // shift each pointer from pos above back, removing the pointer
    for(int i = pos; i < cursor->size + 1; i++){
        cursor->ptr[i] = cursor->ptr[i+1];
    }
    // update new size
    cursor->size--;
    // Verify if new size satisfies minimum size. 
    // Minimum size is irrelevant if node is root.
    if(cursor->size >= (MAX+1)/2-1 || cursor == root )
    {
        cout<<"Deleted "<< x.key_value << " " <<" from internal node successfully\n";
        return;
    }

    cout<<"Underflow in internal node!\n";
    // when reach here, size is too small(underflow)

    // ignore minimum size if node is root
    // if(cursor == root){
    //     return;
    // }
    // Try to get transfer keys from sibling nodes.

    // Get parent of current node.
    Node* parent = findParent(root, cursor);

    int leftSibling, rightSibling;

    // get position of node in parent
    // for(pos = 0; pos < parent->size+1; pos++){
    //     if(parent->ptr[pos] == cursor){
    //         // locate the nearest siblings
    //         leftSibling = pos - 1;
    //         rightSibling = pos + 1;
    //         break;
    //     }
    // }
    pos = 0;
    while (pos < parent->size + 1 && parent->ptr[pos] != cursor) pos++;
    leftSibling = pos - 1;
    rightSibling = pos + 1;
    // If left sibling exists.
    if (leftSibling >= 0)
    {
        Node *leftNode = parent->ptr[leftSibling];
        
        // Can transfer key from left sibling.
        if(leftNode->size >= (MAX+1)/2)
        {
            // Create space for key transfer to current node.
            for(int i = cursor->size; i > 0; i--)
            {
                // Shift all keys and pointers to the right.
                cursor->key[i] = cursor->key[i-1];
                cursor->ptr[i+1] = cursor->ptr[i];
            }
            // Shift remaining pointer.
            cursor->ptr[1] = cursor->ptr[0];

            // Transfer key and pointer from left sibling.
            cursor->key[0] = parent->key[leftSibling];
            cursor->ptr[0] = leftNode->ptr[leftNode->size];

            parent->key[leftSibling] = leftNode->key[leftNode->size-1];

            // //make pointer space for incoming transfer
            // for (int i = cursor->size+1; i > 0; i--){
            //     // shift pointers to the right since incoming pointer is smallest
            //     cursor->ptr[i] = cursor->ptr[i-1];
            // }

            // Update sizes of nodes.
            cursor->size++;
            leftNode->size--;
            cout<<"Transferred "<<cursor->key[0].key_value<<" "<< x.add_vect[0] <<" from left sibling of internal node\n";
            return;
        }
    }
    // Check if right sibling exists.
    if(rightSibling <= parent->size)
    {
        Node *rightNode = parent->ptr[rightSibling];

        // Can transfer key from right node.
        if(rightNode->size >= (MAX+1)/2)
        {
            // Do not need to make space since key to be transferred is largest in cursor.
            // pos represents the index of the pointer that points to the cursor node.
            // Shift leftmost key of right sibling to cursor's last key.
            cursor->key[cursor->size] = parent->key[pos];

            // Update parent key for right sibling. //IS THIS CORRECT WTF TODO
            parent->key[pos] = rightNode->key[0];
            // parent->key[pos] = rightNode->key[1]; ??

            // Shift right sibling keys to the left.
            for (int i = 0; i < rightNode->size -1; i++)
            {
                rightNode->key[i] = rightNode->key[i+1];
                // rightNode->ptr[i] = rightNode->ptr[i+1];
            }

            // Move first pointer of right sibling to cursor's last pointer.
            cursor->ptr[cursor->size+1] = rightNode->ptr[0];
            
            // Shift right sibling pointers to the left.
            for (int i = 0; i < rightNode->size; ++i)
            {
                rightNode->ptr[i] = rightNode->ptr[i+1];
            }
            // Update sizes of nodes.
            cursor->size++;
            rightNode->size--;
            cout<<"Transferred "<<cursor->key[0].key_value << " " << x.add_vect[0] <<" from right sibling of internal node\n";
            return;
        }
    }
    // if we reach here, it means that transferal from sibling nodes was not possible
    // hence we do merging
    // check existence of left sibling
    if(leftSibling >= 0)
    {
        // merged node = left sibling + parent key + cursor
        // get left sibling
        Node *leftNode = parent->ptr[leftSibling];
        // left sibling becomes the merged node
        // left sibling = left sibling + parent key
        leftNode->key[leftNode->size] = parent->key[leftSibling];
        // add each cursor key into left sibling
        for(int i = leftNode->size+1, j = 0; j < cursor->size; j++)
        {
            leftNode->key[i] = cursor->key[j];
        }
        // add each cursor pointer into left sibling
        for(int i = leftNode->size+1, j = 0; j < cursor->size+1; j++)
        {
            leftNode->ptr[i] = cursor->ptr[j];
            cursor->ptr[j] = NULL;
        }
        // update merged node size
        leftNode->size += cursor->size+1;
        // update cursor size
        cursor->size = 0;
        //delete cursor
        removeInternal(parent->key[leftSibling], parent, cursor);
        cout<<"Merged with left sibling\n";

    }
        // since can't merge with left, merge right
        // check existence of right sibling
    else if(rightSibling <= parent->size)
    {
        //merged node = cursor + parent key + right sibling
        Node *rightNode = parent->ptr[rightSibling];
        // cursor becomes the merged node
        // cursor = cursor + parent key
        cursor->key[cursor->size] = parent->key[rightSibling-1];
        // add each right sibling key into cursor
        for(int i = cursor->size+1, j = 0; j < rightNode->size; j++){
            cursor->key[i] = rightNode->key[j];
        }
        // add each right sibling pointer into cursor
        for(int i = cursor->size+1, j = 0; j < rightNode->size+1; j++){
            cursor->ptr[i] = rightNode->ptr[j];
            rightNode->ptr[j] = NULL;
        }
        // update merged node size
        cursor->size += rightNode->size+1;
        // update right sibling size
        rightNode->size = 0;
        //delete right sibling
        removeInternal(parent->key[rightSibling-1], parent, rightNode);
        cout<<"Merged with right sibling\n";
    }
}


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

    if (cursor == root) // if it is root node, then make all pointers NULL IDK WHY THOUGH
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
      cout << "Deleted 1" << endl;
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

      // delete parent node key
      removeInternal(parent->key[rightSibling - 1], parent, rightNode); 
      delete[] rightNode->key;
      delete[] rightNode->ptr;
      delete rightNode;
      cout << "Deleted 1 " << endl;
    }
  }
}
