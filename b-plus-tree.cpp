// int MAX; //size of each node
#include "b_plus_tree.h"
#include <queue>
using namespace std;
extern void *startAddress;

Node::Node() {
  // dynamic memory allocation
  key = new keys_struct[MAX];
  ptr = new Node *[MAX + 1];
  isLeaf = true;
}
/**
 * @brief 
 * 
 * @param x The key of the new leaf node
 * @param cursor 
 * @param child 
 */
void BPTree::insertInternal(keys_struct x, Node* cursor, Node* child)
{
    // cout << cursor << endl;
    // cout << root << endl;
    // cout << "Passed to internal: " << x.key_value << endl;
    //  cout << "Child first key " << child->key[0].key_value << endl;
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
        for (j = MAX+1; j > i; j--)
        {
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
        // cout << "Temp keys!" << endl;
        // for (int i = 0; i < MAX + 1; i++)
        // {
        //     cout << tempKeys[i].key_value << ",";
        // }

        // cout << "Temp pointers" << endl;
        // for (int i = 0; i < MAX + 2; i++)
        // {
        //     cout << tempPointers[i]->key[0].key_value << ",";
        // }
        // cout << endl;

        // split cursor to cursor and newInternal with half of original size
        cursor->size = (MAX+1)/2;
        newInternal->size = MAX-(MAX+1)/2;

        for (i = 0; i< cursor->size; i++)
        {
            cursor->key[i] = tempKeys[i];
            cursor->ptr[i] = tempPointers[i];
        }
        cursor->ptr[cursor->size] = tempPointers[i];
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
        // cout << "First node keys" << endl;
        // for (int i = 0; i < cursor->size; i++)
        // {
        //     cout << cursor->key[i].key_value << ",";
        // }
        //  cout << "First node pointers" << endl;
        // for (int i = 0; i < cursor->size + 1; i++)
        // {
        //     cout << cursor->ptr[i]->key[0].key_value << ",";
        // }

        // cout << "Second node keys" << endl;
        // for (int i = 0; i < newInternal->size; i++)
        // {
        //     cout << newInternal->key[i].key_value << ",";
        // }
        //  cout << "Second node pointers" << endl;
        // for (int i = 0; i < newInternal->size + 1; i++)
        // {
        //     cout << newInternal->ptr[i]->key[0].key_value << ",";
        // }

        if (root == cursor)
        {
            //creation of new node when splitting root node
            Node* newRoot = new Node;
            numNode ++;
            newRoot->key[0] = newInternal->ptr[0]->key[0];
            // cout << "New root key: " << newRoot->key[0].key_value << endl;
            newRoot->ptr[0] = cursor;
            // cout << "New root pointer 0 " << newRoot->ptr[0]->key[0].key_value << endl;
            newRoot->ptr[1] = newInternal;
           //   cout << "New root pointer 1 " << newRoot->ptr[1]->key[0].key_value << endl;
            newRoot->isLeaf = false;
            newRoot->size = 1;
            root = newRoot;
            //cout<<"Created new root\n";
        }
        else
        {   // recursive DFS to find parent node
            insertInternal(newInternal->ptr[0]->key[0], findParent(root, cursor), newInternal);
        }
    }
}


/**
 * @brief 
 * 
 * @param x 
 * @param newKey 
 */
void BPTree::insert(keys_struct x) {
//   cout << root << endl;
  // insert logic
  if (root == nullptr) 
  {
    root = new Node;
    root->key[0] = x;
    root->isLeaf = true;
    root->size = 1;
    numNode++;
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
            while (x.key_value >= cursor->key[i].key_value) i++;
        }
        parent = cursor;
        cursor = cursor->ptr[i];
    }
    // Cursor is now the leaf node in which the new key will be inserted.
    // If there is space to insert the new key.
    if (cursor->size < MAX) {
         //cout <<  "LESS THAN MAX!" <<endl;
      int i = 0;

      // Continue iterating while cursor has not reached the last key and the key to be inserted is larger
      // than the current key.
      while (i < cursor->size && x.key_value > cursor->key[i].key_value) {
          i++;
      }
      if (cursor->key[i].key_value == x.key_value)
      {
        // Insert into LL
        //  cout << cursor->key[i].key_value <<  " already exists." <<endl;
         cursor->key[i].add_vect.push_back(x.add_vect[0]);
        //  cout << "Number of keys: " << cursor->key[i].add_vect.size() <<endl;
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
            // cout <<  "MORE THAN MAX!" <<endl;
      Node *newLeaf = new Node;
      newLeaf->isLeaf = true;

      // Create a temp node to accommodate all the keys and insert x into it
      keys_struct tempNode[MAX + 1];
      for (int i = 0; i < MAX; i++) { //cursor-size == MAX
        tempNode[i] = cursor->key[i];
        // Copy pointers????
      }

      // Find the point to insert the new key.
      if (x.key_value > tempNode[MAX - 1].key_value) 
      {
        tempNode[MAX] = x;
        numNode++;

      }
      else 
      {
        int i = 0;
        while (x.key_value > tempNode[i].key_value) i++;

        // Key is already present
        if (cursor->key[i].key_value == x.key_value) 
        {
        //   cout << cursor->key[i].key_value <<  " already exists." <<endl;
          cursor->key[i].add_vect.push_back(x.add_vect[0]);
        //   cout << "Number of keys: " << cursor->key[i].add_vect.size() <<endl;
          return;
        }
        numNode++;

        // cout << x.key_value << " is not present." << endl;

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
      int i;
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

/**
 * @brief 
 * 
 * @param x The key of the node to be removed.
 * @param cursor Parent.
 * @param child The removed node.
 * @param child The retained node.
 * @return int number of nodes that are merged.
 */
int BPTree::removeInternal(keys_struct x, Node* cursor, Node* child, Node* newNode)
{
    cout << "Remove internal node: " << x.key_value << endl;
    // If root node key is being deleted and root is empty after removal, child becomes new root.
    if(cursor == root && cursor->size == 1)
    {
        // check if larger or smaller pointer points to child
        // other pointer that do not point to child becomes new root
        // Check if the adjacent pointers of the first key points to child.
        if (cursor->ptr[0] == child || cursor->ptr[1] == child)
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

            cout << "Deleted 1" << endl;
            // set new root
            // root = cursor->ptr[newRoot]

            // Delete old root node.
            delete[] cursor->key;
            delete[] cursor->ptr;
            delete cursor;
            numNode--;
            cout << "Deleted 1" << endl;
            //cout<<"Changed root node\n";
            return 0;
        }
    }

    cout << "Current cursor keys: " << endl;
    for (int k = 0; k < cursor->size; k++)
    {
        cout << cursor->key[k].key_value << "|";
    }
    cout << endl;
    cout << "Current cursor pointers: " << endl;
    for (int k = 0; k < cursor->size + 1; k++)
    {
        cout << cursor->ptr[k]->key[0].key_value << "|";
    }
    cout << endl;
    // Search for the key that is to be deleted.
    int pos = 0;
    while (pos < cursor->size && x.key_value != cursor->key[pos].key_value) pos++;

    // Shift each key from pos above back, removing the key
    for (int i = pos; i < cursor->size; i++)
    {
        cursor->key[i] = cursor->key[i+1];
    }
    // Search for the position of the pointer that is to be deleted.
    pos = 0;
    while (pos < cursor->size + 1 && cursor->ptr[pos] != child) pos++;
    // delete[] child->key;
    // delete[] child->ptr;
    // delete child;

    cout << "Pos of pointer pointing to child node to be deleted: " << pos << endl;
    // Shift each pointer to the left to remove pointer.
    for(int i = pos; i < cursor->size + 1; i++)
    {
        cursor->ptr[i] = cursor->ptr[i+1];
    }
    cursor->ptr[pos-1] = newNode; //TODO CHECK THIS (ADDED)
    // Update size of parent after removing key.
    cursor->size--;

    // Verify if new size satisfies minimum size. 
    // Minimum size is irrelevant if node is root.
    if (cursor->size >= (MAX+1)/2-1 || cursor == root )
    {
        cout<<"Deleted "<< x.key_value << " " <<" from internal node successfully\n";
        return 0;
    }

    cout<< "Internal node has insufficient keys!" << endl;

    cout << "Current cursor keys: " << endl;
    for (int k = 0; k < cursor->size; k++)
    {
        cout << cursor->key[k].key_value << "|";
    }
    cout << endl;
    cout << "Current cursor pointers: " << endl;
    for (int k = 0; k < cursor->size + 1; k++)
    {
        cout << cursor->ptr[k]->key[0].key_value << "|";
    }
    cout << endl;
    // when reach here, size is too small(underflow)

    // ignore minimum size if node is root
    // if(cursor == root){
    //     return;
    // }
    // Try to get transfer keys from sibling nodes.

    // Get parent of internal node.
    Node* parent = findParent(root, cursor);

    int leftSibling, rightSibling;

    // Get index of pointer to the node in parent node.
    pos = 0;
    while (pos < parent->size + 1 && parent->ptr[pos] != cursor) pos++;
    leftSibling = pos - 1;
    rightSibling = pos + 1;


    // If left sibling exists, transfer key from left sibling.
    if (leftSibling >= 0)
    {
        cout << "Merge with left sibling!" << endl;
        Node *leftNode = parent->ptr[leftSibling];
        
        // Can transfer key from left sibling.
        if (leftNode->size >= (MAX+1)/2)
        {
            // Create space for key transfer to current node.
            for(int i = cursor->size; i > 0; i--)
            {
                // Shift all keys to the right.
                cursor->key[i] = cursor->key[i-1];
            }

            // Transfer key from left sibling.
            cursor->key[0] = parent->key[leftSibling];
            parent->key[leftSibling] = leftNode->key[leftNode->size-1];

            // Create space for pointer transfer.
            for (int i = cursor->size + 1; i > 0; i--) {
                // Shift all pointers to the right.
                cursor->ptr[i] = cursor->ptr[i - 1];
            }

            // Transfer pointer from left sibling.
            cursor->ptr[0] = leftNode->ptr[leftNode->size];

            // Update sizes of nodes.
            cursor->size++;
            leftNode->size--;
            cout << "Transferred " <<cursor->key[0].key_value<< "from left sibling of node!" << endl;
            return 0;
        }
    }
    // If right sibling exists, try to transfer from right sibling.
    if (rightSibling <= parent->size)
    {
        cout << "Merge with right sibling!" << endl;
        Node *rightNode = parent->ptr[rightSibling];

        // Can transfer key from right node.
        if(rightNode->size >= (MAX+1)/2)
        {
            // Do not need to make space since key to be transferred is largest in cursor.
            // pos represents the index of the pointer that points to the cursor node.
            // Shift leftmost key of right sibling to cursor's last key.
            cursor->key[cursor->size] = parent->key[pos];

            // Shift right sibling keys to the left.
            for (int i = 0; i < rightNode->size -1; i++)
            {
                rightNode->key[i] = rightNode->key[i+1];
            }
            
            // Move first pointer of right sibling to cursor's last pointer.
            cursor->ptr[cursor->size+1] = rightNode->ptr[0];
            
            // Shift right sibling pointers to the left.
            for (int i = 0; i < rightNode->size; ++i)
            {
                rightNode->ptr[i] = rightNode->ptr[i+1];
            }
            // Update parent key for right sibling.
            parent->key[pos] = rightNode->key[0];

            // Update sizes of nodes.
            cursor->size++;
            rightNode->size--;
            cout<<"Transferred  "<<cursor->key[0].key_value <<" from right sibling of node!" << endl;
            return 0;
        }
    }
    // Not possible to transfer from sibling nodes, merge with sibling node.
    if (leftSibling >= 0)
    {
        cout << "Merge with left sibling!" << endl;
        Node *leftNode = parent->ptr[leftSibling];

        // Merge cursor into left sibling.
        // leftNode->key[leftNode->size] = parent->key[leftSibling];
        leftNode->key[leftNode->size] = cursor->ptr[0]->key[0];
        for(int i = leftNode->size+1, j = 0; j < cursor->size; j++)
        {
            leftNode->key[i] = cursor->key[j];
        }
        // Add each pointer from cursor into left sibling.
        for(int i = leftNode->size+1, j = 0; j < cursor->size+1; j++)
        {
            leftNode->ptr[i] = cursor->ptr[j];
            cursor->ptr[j] = nullptr;
        }
        // Update sizes.
        leftNode->size += cursor->size+1;
        cursor->size = 0;

        // Passes the key of the cursor (Key to be removed). 
        int numNodesMerged = removeInternal(parent->key[leftSibling], parent, cursor, leftNode);
        return 1 + numNodesMerged;
    }
    // Try to merge right sibling.
    else if (rightSibling <= parent->size)
    {
        cout<<"Merged with right sibling!" << endl;

        // Merge right sibling into cursor.
        Node *rightNode = parent->ptr[rightSibling];
        cursor->key[cursor->size] = parent->key[rightSibling-1];

        // Add each right sibling key into cursor
        for(int i = cursor->size+1, j = 0; j < rightNode->size; j++)
        {
            cursor->key[i] = rightNode->key[j];
        }
        // Add each right sibling pointer into cursor
        for(int i = cursor->size+1, j = 0; j < rightNode->size+1; j++)
        {
            cursor->ptr[i] = rightNode->ptr[j];
            rightNode->ptr[j] = NULL;
        }
        // Update sizes.
        cursor->size += rightNode->size+1;
        rightNode->size = 0;

        // Passes the key of the right sibling (Key to be removed). 
        int numNodesMerged = removeInternal(parent->key[rightSibling-1], parent, rightNode, cursor);
        return 1 + numNodesMerged;
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

    // Cursor travels to the leaf nodes.
    while (!cursor->isLeaf) {
        parent = cursor;
        int i;
        if (x.key_value >= cursor->key[cursor->size -1].key_value)
        {
            i = cursor->size;
        }
        else {
            i = 0;
            while (x.key_value >= cursor->key[i].key_value ) 
            {
                i++;
            }
            // i is now the pointer index
        }
        leftSibling = i - 1;
        rightSibling = i + 1; // Right sibling cannot be from another parent node.
        cursor = cursor->ptr[i];
    }
    // Search for the key if it exists.
    int pos = 0;
    while (x.key_value > cursor->key[pos].key_value) pos++;
    if (x.key_value != cursor->key[pos].key_value) // if key does not exist in that leaf node
    {
      cout<<"Key does not exist!"<< endl;
      return 0;
    }

    //Delete the key by shifting all elements that are above the key down.
    for (int i = pos; i < cursor->size; i++) {
      cursor->key[i] = cursor->key[i + 1];
    }
    cursor->size--;

    if (cursor == root) // if it is root node, then make all pointers NULL
    {
      cout << "Deleted " << x.key_value << " " << "from leaf node successfully\n";
      for (int i = 0; i < MAX + 1; i++) {
        cursor->ptr[i] = nullptr;
      }
      if (cursor->size == 0) // if all keys are deleted
      {
        // cout<<"Tree died\n";
        delete[] cursor->key;
        delete[] cursor->ptr;
        delete cursor;
        cout << "Deleted 1" << "\n";
        root = nullptr;
        return 1;
      }
      return 0;
    }

    // Shift the pointer to the adjacent leaf node.
    cursor->ptr[cursor->size] = cursor->ptr[cursor->size + 1];
    cursor->ptr[cursor->size + 1] = nullptr;
    cout << "Deleted " << x.key_value << " "  << " from leaf node successfully\n";

    // There are sufficient keys in node.
    if (cursor->size >= (MAX + 1) / 2)
    {
        cout << "Sufficient nodes!" << endl;
      // Update parent if necessary.
      if (pos == 0)
      {
           cout << "Update parent!" << endl;
        parent->key[leftSibling] = cursor->key[0];
      }
      return 0;
    }

    cout << "Underflow condition!" << endl;
    // Underflow condition. 
    // Try to transfer a key from sibling node.
    // Check if left sibling exists.
    if (leftSibling >= 0) 
    {
      cout << "Shift from left sibling!" << endl;
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
        return 0;
      }
    }
    // Check if right sibling exists.
    if (rightSibling <= parent->size)
    {
        cout << "Shift from right sibling!" << endl;
      Node *rightNode = parent->ptr[rightSibling];
      // Check if it is possible to transfer.
      if (rightNode->size >= (MAX + 1) / 2 + 1) {

        // shift pointer to next leaf
        cursor->size++;
        cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
        cursor->ptr[cursor->size - 1] = nullptr;
        // transfer
        cursor->key[cursor->size - 1] = rightNode->key[0];

        // Update pointer of right sibling.
        rightNode->size--;
        rightNode->ptr[rightNode->size] = rightNode->ptr[rightNode->size + 1];
        rightNode->ptr[rightNode->size + 1] = nullptr;
        
        // Shift keys of right sibling down by 1.
        for (int i = 0; i < rightNode->size; i++) 
        {
          rightNode->key[i] = rightNode->key[i + 1];
        }
        // update parent
        parent->key[rightSibling - 1] = rightNode->key[0];
        return 0;
      }
    }
    // If left sibling exists, merge with it.
    if (leftSibling >= 0) 
    {
      cout << "Merged with left sibling!" << endl;
      Node *leftNode = parent->ptr[leftSibling];
      
      // Transfer all keys to left sibling.
      for (int i = leftNode->size, j = 0; j < cursor->size; i++, j++) 
      {
        leftNode->key[i] = cursor->key[j];
      }
      leftNode->ptr[leftNode->size] = nullptr;
      leftNode->size += cursor->size;

      // Transfer pointer to adjacent leaf node.
      leftNode->ptr[leftNode->size] = cursor->ptr[cursor->size];

      // Delete parent node key.
      int numNodeMerged = removeInternal(parent->key[leftSibling], parent, cursor, leftNode);
      
      delete[] cursor->key;
      delete[] cursor->ptr;
      delete cursor;
      cout << "Deleted 1" << endl;
      return 1 + numNodeMerged;
    } 
    // Else if right sibling exists, merge with it.
    else if (rightSibling <= parent->size)
    {
      cout << "Merged with right sibling!" << endl;
      Node *rightNode = parent->ptr[rightSibling];

      // Transfer all keys to cursor.
      for (int i = cursor->size, j = 0; j < rightNode->size; i++, j++) {
        cursor->key[i] = rightNode->key[j];
      }

      // Transfer pointer to adjacent leaf node.
      cursor->ptr[cursor->size] = nullptr;
      cursor->size += rightNode->size;
      cursor->ptr[cursor->size] = rightNode->ptr[rightNode->size];

      // Delete parent node key.
      int numNodesMerged = removeInternal(parent->key[rightSibling - 1], parent, rightNode, cursor); 
      delete[] rightNode->key;
      delete[] rightNode->ptr;
      delete rightNode;
      cout << "Deleted 1 " << endl;
      return 1 + numNodesMerged;
    }
  }
}

Node *BPTree::findParent(Node *cursor, Node *child) {
  // find parent of child node within cursor use DFS
  // returns NULL if child has no parent in cursor
  Node *parent;
  // skip search if cursor is on last/second last level
  // cursor cannot be a parent if it is leaf node
  // cursor cannot be second last level since function is not called when child
  // node is leaf node during insertion
  if (cursor->isLeaf || (cursor->ptr[0])->isLeaf) {
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

BPTree::BPTree() : root{NULL} {}

void BPTree::searchRange(float lowerBoundKey, float upperBoundKey) 
{
    int numIndexNodes = 0;
    if (root == nullptr) 
    {
        throw std:: logic_error("Tree is empty!");
    } 
    else {
        Node *cursor = root;
        //While leaf node is not reached yet.
        int level = 0;
        while (!cursor->isLeaf) 
        {
            cout << "Index node at level: " << level << endl;
            for (int k = 0; k < cursor->size; k++)
            {
                cout << cursor->key[k].key_value << "|";
            }
            cout << endl;
            numIndexNodes++;
            // Iterate through 
            if (lowerBoundKey >= cursor->key[cursor->size-1].key_value) 
            {
                cursor = cursor->ptr[cursor->size];
            } 
            else 
            {
                int i = 0;
                while (lowerBoundKey >= cursor->key[i].key_value) i++;
                cursor = cursor->ptr[i];
           }
           level++;
        }
        cout << "Number of index nodes: " << numIndexNodes << endl;
        //Here, we are at the leaf nodes. We then traverse through the nodes from the lowerBoundKey to the upperBoundKey.
        int i = 0;
        // cout << "Key index: " << cursor->key[i].key_value << endl;
        while (cursor->key[i].key_value < lowerBoundKey) i++;
        // cout << "Key index: " << cursor->key[i].key_value << endl;
        float totalRating = 0;
        int totalRecords = 0;
        // cout << "Leaf nodes accessed: " << numIndexNodes << endl;
        // for (int k = 0; k < cursor->size; k++)
        // {
        //      cout << cursor->key[k].key_value << "|";
        // }
        while (cursor->key[i].key_value <= upperBoundKey) 
        {
            // cout << "Record value: " << cursor->key[i].key_value << endl;
            // cout << "Number of records: " << cursor->key[i].add_vect.size() << endl;
            totalRecords += cursor->key[i].add_vect.size();
            for (int j = 0; j < cursor->key[i].add_vect.size(); ++j) {
                //printf("Data Block: ");
                //printf("%p", (uchar *) cursor->key[i].add_vect[j]);
                //printf("\n");
                //printf("tconst: ");
                totalRating +=(*(Record *)cursor->key[i].add_vect[j]).averageRating;
            }
            if (i == cursor->size - 1 && cursor->ptr[cursor->size] != NULL && cursor->key[i].key_value <= upperBoundKey) {
                cursor = cursor->ptr[cursor->size];
                // cout << endl; 
                // for (int k = 0; k < cursor->size; k++)
                // {
                //      cout << cursor->key[k].key_value << "|";
                // }
                i = 0;
                continue;
            } 
            i++;
        }
        cout << "Average Rating: " << totalRating / totalRecords << endl;
    }
}


void BPTree::searchSingle(float key) 
{
    int numIndexNodes = 0;
    if (root == nullptr) 
    {
        throw std:: logic_error("Tree is empty!");
    } 
    else {
        Node *cursor = root;
        //While leaf node is not reached yet.
        int level = 0;
        while (!cursor->isLeaf) 
        {
            cout << "Index node at level: " << level << endl;
            for (int k = 0; k < cursor->size; k++)
            {
                cout << cursor->key[k].key_value << "|";
            }
            cout << endl;
            numIndexNodes++; 
            // Iterate through 
            if (key >= cursor->key[cursor->size-1].key_value) 
            {
                cursor = cursor->ptr[cursor->size];
            } 
            else 
            {
                int i = 0;
                while (key >= cursor->key[i].key_value) i++;
                cursor = cursor->ptr[i];
           }
           level++;
        }
        cout << "Number of index nodes: " << numIndexNodes << endl;
        int i = 0;
        // cout << "Key index: " << cursor->key[i].key_value << endl;
        while (cursor->key[i].key_value < key) i++;
        // cout << "Key index: " << cursor->key[i].key_value << endl;
        float totalRating = 0;
        if (cursor->key[i].key_value == key) 
        {
            cout << "Number of records: " << cursor->key[i].add_vect.size() << endl;
            for (int j = 0; j < cursor->key[i].add_vect.size(); ++j) {
                //printf("Data Block: ");
                //printf("%p", (uchar *) cursor->key[i].add_vect[j]);
                //printf("\n");
                //printf("tconst: ");
                //cout << (*(Record *) cursor->key[i].add_vect[j]).tconst << "\n";
                totalRating +=(*(Record *)cursor->key[i].add_vect[j]).averageRating;
            }
        }
        cout << "Average Rating: " << totalRating / cursor->key[i].add_vect.size() << endl;
    }
}

// recursively traverse tree to find height
int BPTree::height(Node *cursor) {
 if (cursor->isLeaf) {
    // reached leaf
    return 1;
  } 
  return height(cursor->ptr[0]) + 1;
}


int BPTree::display(Node *cursor, int nodecount, bool first) {
  if (first == true) {
    for (int i = 0; i < cursor->size + 1; i++) {
      if (i != cursor->size) {
        cout << 0 << " " << cursor->key[i].key_value << "\n";
      }
      cout << 1 << "\n";
      // cout << cursor->ptr[i]->size << " Size \n";
      for (int j = 0; j < cursor->ptr[i]->size; j++) {
        cout << cursor->ptr[i]->key[j].key_value << " ";
      }
      cout << "\n";
    }
    return 0;
  } else {
    int thisnode;
    if (cursor->isLeaf == false) {
      for (int i = 0; i < cursor->size + 1; i++) {
        thisnode = display(cursor->ptr[i], 0, false);
        nodecount = nodecount + thisnode;
        // cout<<"\n"<<nodecount<<"\n";

        // read(curr->child_ptr[i]);
      }
      nodecount++;
      return nodecount;
    }
    if (cursor->isLeaf == true) {

      for (int i = 0; i < cursor->size; i++) {
        cout << cursor->key[i].key_value << " ";
      }
      return 1;
    }
    return 0;
  }
}

void BPTree::printTree(Node *cursor) {
 // Base Case
    if (cursor == NULL)  return;
 
    // Create an empty queue for level order traversal
    queue<Node *> q;
 
    // Enqueue Root and initialize height
    q.push(cursor);
 
    while (!q.empty())
    {
        int queueSize = q.size();
        for (int i = 0; i < queueSize; i++)
        {
            // Print front of queue and remove it from queue
            Node *node = q.front();
            for (int i = 0; i < node->size; i++)
            {
                cout << node->key[i].key_value << "|";
                if (!node->isLeaf)
                {
                    q.push(node->ptr[i]);
                }
            }
            cout << "-----";
            if (!node->isLeaf)
            {
              q.push(node->ptr[node->size]);  
            }
            q.pop();
        }
        cout << endl;
    }
}

int BPTree::getMax() { return MAX; }

Node *BPTree::getRoot() { return root; }
