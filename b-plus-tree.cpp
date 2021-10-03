#include "b-plus-tree.h"
#include <bits/stdc++.h>
#include <queue>
#include <unordered_map>
using namespace std;
extern void *startAddress;
extern uint blockSize;

Node::Node()
{
  // dynamic memory allocation
  keys = new keys_struct[MAX];
  pointers = new Node *[MAX + 1];
  isLeaf = true;
}

/**
 * @brief
 * Inserts a new internal node in tree.
 * @param x The key to be added to the parent internal node.
 * @param cursor The parent.
 * @param child The new child node.
 */
void BPTree::insertInternal(keys_struct x, Node *cursor, Node *child)
{
  // If there is space in cursor, insert into cursor.
  if (cursor->size < MAX)
  {
    // Search for position of new key.
    int i = 0;
    while (i < cursor->size && x.key_value > cursor->keys[i].key_value)
      i++;

    // Make space for new key.
    for (int j = cursor->size; j > i; j--)
    {
      cursor->keys[j] = cursor->keys[j - 1];
      cursor->pointers[j + 1] = cursor->pointers[j];
    }
    cursor->keys[i] = x;             // Set new key.
    cursor->size++;                  // Set new size.
    cursor->pointers[i + 1] = child; // Set new pointer.
  }
  else
  {
    // create another internal node when overflow
    Node *newInternal = new Node;
    numNode++;
    newInternal->isLeaf = false;

    // Create temporary node with extra key and pointer to store keys and
    // pointers.
    keys_struct tempKeys[MAX + 1];

    // MAX refers to the number of keys, pointer = number of keys + 1
    Node *tempPointers[MAX + 2];

    // Copy keys and nodes to the temporary node.
    for (int i = 0; i < MAX; i++)
    {
      tempKeys[i] = cursor->keys[i];
      tempPointers[i] = cursor->pointers[i];
    }
    tempPointers[MAX] = cursor->pointers[MAX];

    int j;
    int i = 0;

    // Search for position of new key.
    while (i < MAX && x.key_value > tempKeys[i].key_value)
    {
      i++;
    }

    // Create space for new key.
    for (j = MAX; j > i; j--)
    {
      tempKeys[j] = tempKeys[j - 1];
      tempPointers[j + 1] = tempPointers[j];
    }
    // Insert new key.
    tempKeys[i] = x;

    // Insert new pointer.
    tempPointers[i + 1] = child;

    // Split cursor into two nodes.
    cursor->size = (MAX + 1) / 2;
    newInternal->size = MAX - (MAX + 1) / 2;

    for (i = 0; i < cursor->size; i++)
    {
      cursor->keys[i] = tempKeys[i];
      cursor->pointers[i] = tempPointers[i];
    }
    cursor->pointers[cursor->size] = tempPointers[i];

    // Set keys and pointers of new node.
    // Skip the first key of the new internal node.
    for (i = 0, j = cursor->size + 1; i < newInternal->size; i++, j++)
    {
      newInternal->keys[i] = tempKeys[j];
      newInternal->pointers[i] = tempPointers[j];
    }
    newInternal->pointers[newInternal->size] = tempPointers[j];

    if (root == cursor)
    {
      Node *newRoot = new Node;
      numNode++;
      // New key that should be added in the root node.
      keys_struct newKey = getNewKey(newInternal);
      newRoot->keys[0] = newKey;
      newRoot->pointers[0] = cursor;
      newRoot->pointers[1] = newInternal;
      newRoot->isLeaf = false;
      newRoot->size = 1;
      root = newRoot;
      cout << "Created new root!" << endl;
    }
    else
    { // New key that should be added in the parent node.
      keys_struct newKey = getNewKey(newInternal);
      insertInternal(newKey, findParent(root, cursor), newInternal);
    }
  }
}

/**
 * @brief
 * Get the key that is the smallest on the right subtree.
 * @param cursor Current node
 * @return keys_struct the smallest key
 */
keys_struct BPTree::getNewKey(Node *cursor)
{
  while (!cursor->isLeaf)
  {
    cursor = cursor->pointers[0];
  }
  return cursor->keys[0];
}

/**
 * @brief
 * Insert key into tree.
 * @param x Key to be inserted.
 */
void BPTree::insert(keys_struct x)
{
  if (root == nullptr)
  {
    root = new Node;
    root->keys[0] = x;
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
      parent = cursor;
      if (x.key_value >= cursor->keys[cursor->size - 1].key_value)
      {
        cursor = cursor->pointers[cursor->size];
      }
      else
      {
        // // Iterate through the keys to find the relevant key/ptr
        int i = 0;
        while (x.key_value >= cursor->keys[i].key_value)
          i++;
        cursor = cursor->pointers[i];
      }
    }
    // Cursor is now the leaf node in which the new key will be inserted.
    // If there is space to insert the new key.
    for (int k = 0; k < cursor->size; k++)
    {
      if (cursor->keys[k].key_value == x.key_value)
      {
        cursor->keys[k].add_vect.push_back(x.add_vect[0]);
        return;
      }
    }
    // There is space for new element.
    if (cursor->size < MAX)
    {
      int i = 0;

      // Continue iterating while cursor has not reached the last key and the
      // key to be inserted is larger than the current key.
      if (x.key_value > cursor->keys[cursor->size - 1].key_value)
      {
        i = cursor->size;
      }
      while (i < cursor->size && x.key_value > cursor->keys[i].key_value)
      {
        i++;
      }
      // Make space for new key.
      for (int j = cursor->size; j > i; j--)
      {
        cursor->keys[j] = cursor->keys[j - 1];
      }

      // Insert key into the node.
      cursor->keys[i] = x;
      cursor->size++;
      cursor->pointers[cursor->size] = cursor->pointers[cursor->size - 1];
      cursor->pointers[cursor->size - 1] = NULL;
      return;
    }
    // Insufficient space to insert new key.
    else
    {
      Node *newLeaf = new Node;
      newLeaf->isLeaf = true;

      // Create a temp node to accommodate all the keys and insert x into it
      keys_struct tempNode[MAX + 1];
      for (int i = 0; i < MAX; i++)
      {
        tempNode[i] = cursor->keys[i];
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
        while (x.key_value > tempNode[i].key_value)
          i++;
        numNode++;

        // Create space for new key in virtual node.
        for (int j = MAX; j > i; j--)
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
      cursor->pointers[cursor->size] = newLeaf;

      // Set the rightmost pointer of the new leaf node to the next leaf node.
      newLeaf->pointers[newLeaf->size] = cursor->pointers[MAX];
      cursor->pointers[MAX] = nullptr;

      // Distribute elements to the two new leaf nodes.
      int i;
      for (i = 0; i < cursor->size; i++)
      {
        cursor->keys[i] = tempNode[i];
      }
      for (int j = 0; j < newLeaf->size; i++, j++)
      {
        newLeaf->keys[j] = tempNode[i];
      }

      // Modify the parent node.
      if (cursor == root)
      {
        // if cursor is a root node, we create a new root.
        Node *newRoot = new Node;
        numNode++;

        // Set the new root's key to be the left bound of the right child.
        newRoot->keys[0] = newLeaf->keys[0];
        newRoot->pointers[0] = cursor;
        newRoot->pointers[1] = newLeaf;

        // Update attributes of new root.
        newRoot->isLeaf = false;
        newRoot->size = 1;
        root = newRoot;
      }
      else
      {
        // Insert first key of new leaf intp parent node.
        insertInternal(newLeaf->keys[0], parent, newLeaf);
      }
    }
  }
}

/**
 * @brief
 * Remove internal node.
 * @param x The key of the node to be removed.
 * @param cursor Parent.
 * @param child The removed node.
 * @return int number of nodes that are merged.
 */
int BPTree::removeInternal(keys_struct x, Node *cursor, Node *child)
{
  cout << "Remove internal node: " << x.key_value << endl;
  // If root node key is being deleted and root is empty after removal, child
  // becomes new root.
  if (cursor == root && cursor->size == 1)
  {
    // Check if the adjacent pointers of the first key points to child.
    // Other pointer that does not point to child becomes new root.
    if (cursor->pointers[0] == child || cursor->pointers[1] == child)
    {
      // Set new root.
      root = cursor->pointers[0] == child ? cursor->pointers[1] : cursor->pointers[0];

      // Delete child node.
      delete[] child->keys;
      delete[] child->pointers;
      delete child;
      numNode--;

      cout << "Deleted 1" << endl;

      // Delete old root node.
      delete[] cursor->keys;
      delete[] cursor->pointers;
      delete cursor;
      numNode--;
      cout << "Deleted 1" << endl;

      cout << "Root node changed." << endl;
      return 0;
    }
  }
  // Search for the key that is to be deleted.
  int keyPos = 0;
  while (keyPos < cursor->size && x.key_value != cursor->keys[keyPos].key_value)
    keyPos++;

  // Delete the key by shifting all keys forward.
  for (int i = keyPos; i < cursor->size - 1; i++) // cursor->size - 1 ??
  {
    cursor->keys[i] = cursor->keys[i + 1];
  }
  // Search for the position of the pointer that is to be deleted.
  int ptrPos = 0;
  while (ptrPos < cursor->size + 1 && cursor->pointers[ptrPos] != child)
    ptrPos++;

  // Shift each pointer to the left to remove pointer.
  for (int i = ptrPos; i < cursor->size; i++) //cursor size + 1/ cursor size??
  {
    cursor->pointers[i] = cursor->pointers[i + 1];
  }

  // Update size of parent after removing key.
  cursor->size--;

  // Verify if new size satisfies minimum size.
  // Minimum size is irrelevant if node is root.
  if (cursor->size >= (MAX + 1) / 2 - 1 || cursor == root)
  {
    cout << "Deleted " << x.key_value << " "
         << " from internal node successfully\n";
    numNode--;
    return 0;
  }
  // Here, the new node size is insufficient.
  // Try to get transfer keys from sibling nodes.

  // Get parent of internal node.
  Node *parent = findParent(root, cursor);

  int leftSiblingPtr, rightSiblingPtr;

  // Get index of pointer to the node in parent node.
  int pos = 0;
  while (pos < parent->size + 1 && parent->pointers[pos] != cursor)
    pos++;
  leftSiblingPtr = pos - 1;
  rightSiblingPtr = pos + 1;

  // If left sibling exists, transfer key from left sibling.
  if (leftSiblingPtr >= 0)
  {
    cout << "Merge with left sibling!" << endl;
    Node *leftNode = parent->pointers[leftSiblingPtr];

    // Can transfer key from left sibling.
    if (leftNode->size >= (MAX + 1) / 2)
    {
      // Create space for key transfer to current node.
      for (int i = cursor->size; i > 0; i--)
      {
        // Shift all keys to the right.
        cursor->keys[i] = cursor->keys[i - 1];
      }

      // Create space for pointer transfer.
      for (int i = cursor->size + 1; i > 0; i--)
      {
        // Shift all pointers to the right.
        cursor->pointers[i] = cursor->pointers[i - 1];
      }

      // Transfer key from left sibling.
      // parent->key[leftSiblingPtr] is the lowest key for the current subtree.
      cursor->keys[0] = parent->keys[leftSiblingPtr];

      //leftNode->key[leftNode->size - 1] is the smallest key of the node subtree that is being transferred.
      parent->keys[leftSiblingPtr] = leftNode->keys[leftNode->size - 1];

      // Transfer pointer from left sibling.
      cursor->pointers[0] = leftNode->pointers[leftNode->size];

      // Update sizes of nodes.
      cursor->size++;
      leftNode->size--;
      cout << "Transferred " << cursor->keys[0].key_value
           << "from left sibling of node!" << endl;
      return 0;
    }
  }
  // If right sibling exists, try to transfer from right sibling.
  if (rightSiblingPtr <= parent->size)
  {
    cout << "Merge with right sibling!" << endl;
    Node *rightNode = parent->pointers[rightSiblingPtr];

    // Can transfer key from right node.
    if (rightNode->size >= (MAX + 1) / 2)
    {
      // Do not need to make space since key to be transferred is largest in
      // cursor.
      // pos represents the index of the pointer that points to the cursor node.
      // parent->key[pos] is the lowest key of the right sibling's subtree.
      cursor->keys[cursor->size] = parent->keys[pos];

      // Update parent key for right sibling.
      parent->keys[pos] = rightNode->keys[0];

      // Shift right sibling keys to the left to remove that key.
      for (int i = 0; i < rightNode->size - 1; i++)
      {
        rightNode->keys[i] = rightNode->keys[i + 1];
      }

      // Move first pointer of right sibling to cursor's last pointer.
      cursor->pointers[cursor->size + 1] = rightNode->pointers[0];

      // Shift right sibling pointers to the left.
      for (int i = 0; i < rightNode->size; i++)
      {
        rightNode->pointers[i] = rightNode->pointers[i + 1];
      }

      // Update sizes of nodes.
      cursor->size++;
      rightNode->size--;
      cout << "Transferred  " << cursor->keys[0].key_value
           << " from right sibling of node!" << endl;
      return 0;
    }
  }
  // Not possible to transfer from sibling nodes, merge with sibling node.
  if (leftSiblingPtr >= 0)
  {
    cout << "Merge with left sibling!" << endl;
    Node *leftNode = parent->pointers[leftSiblingPtr];

    // Merge cursor into left sibling.
    // parent->key[leftSiblingPtr] is the smallest key in the cursor subtree to be merged in.
    leftNode->keys[leftNode->size] = parent->keys[leftSiblingPtr];

    // Add each remaining key into left sibling.
    for (int i = leftNode->size + 1, j = 0; j < cursor->size; j++)
    {
      leftNode->keys[i] = cursor->keys[j];
    }
    // Add each pointer from cursor into left sibling.
    for (int i = leftNode->size + 1, j = 0; j <= cursor->size; j++)
    {
      leftNode->pointers[i] = cursor->pointers[j];
      cursor->pointers[j] = nullptr;
    }
    // Update sizes.
    // This is cursor + 1 due to the additional key that is added which was previously the smallest key of the cursor.
    leftNode->size += cursor->size + 1;
    cursor->size = 0;

    // Passes the key of the cursor (Key to be removed).
    int numNodesMerged =
        removeInternal(parent->keys[leftSiblingPtr], parent, cursor);
    return 1 + numNodesMerged;
  }
  // Try to merge right sibling.
  else if (rightSiblingPtr <= parent->size)
  {
    cout << "Merged with right sibling!" << endl;

    // Merge right sibling into cursor.
    Node *rightNode = parent->pointers[rightSiblingPtr];

    // parent->key[rightSiblingPtr - 1] is the smallest key in the right subtree to be merged in.
    cursor->keys[cursor->size] = parent->keys[rightSiblingPtr - 1];

    // Add each right sibling key into cursor
    for (int i = cursor->size + 1, j = 0; j < rightNode->size; j++)
    {
      cursor->keys[i] = rightNode->keys[j];
    }
    // Add each right sibling pointer into cursor
    for (int i = cursor->size + 1, j = 0; j <= rightNode->size; j++)
    {
      cursor->pointers[i] = rightNode->pointers[j];
      rightNode->pointers[j] = NULL;
    }
    // Update sizes.
    // This is right node + 1 due to the additional key that is added which was previously the smallest key of the right node.
    cursor->size += rightNode->size + 1;
    rightNode->size = 0;

    // Passes the key of the right sibling (Key to be removed).
    int numNodesMerged = removeInternal(parent->keys[rightSiblingPtr - 1], parent,
                                        rightNode);
    return 1 + numNodesMerged;
  }
}

/**
 * @brief
 * Remove key from tree.
 * @param x Key to be removed.
 * @return int Number of removed nodes.
 */
int BPTree::remove(keys_struct x)
{
  if (root == nullptr)
  {
    throw std::logic_error("Tree is empty!");
  }
  else
  {
    Node *cursor = root;
    Node *parent;

    int leftSiblingPtr, rightSiblingPtr;

    // Cursor travels to the leaf nodes.
    while (!cursor->isLeaf)
    {
      parent = cursor;
      int i;
      if (x.key_value >= cursor->keys[cursor->size - 1].key_value)
      {
        i = cursor->size;
      }
      else
      {
        i = 0;
        while (x.key_value >= cursor->keys[i].key_value)
        {
          i++;
        }
      }
      leftSiblingPtr = i - 1;
      rightSiblingPtr = i + 1;
      cursor = cursor->pointers[i];
    }
    // Search for the key if it exists.
    int pos = 0;
    while (x.key_value > cursor->keys[pos].key_value)
      pos++;
    // Key does not exist in the tree.
    if (x.key_value != cursor->keys[pos].key_value)
    {
      cout << "Key does not exist!" << endl;
      return 0;
    }

    // Delete the key by shifting all elements that are above the key down.
    for (int i = pos; i < cursor->size; i++)
    {
      cursor->keys[i] = cursor->keys[i + 1];
    }
    cursor->size--;

    if (cursor == root) // If it is root node, make all pointers null.
    {
      cout << "Deleted " << x.key_value << " "
           << "from leaf node successfully" << endl;
      ;
      numNode--;
      for (int i = 0; i < MAX + 1; i++)
      {
        cursor->pointers[i] = nullptr;
      }
      if (cursor->size == 0) // If all keys are deleted.
      {
        cout << "Tree is deleted!" << endl;
        delete[] cursor->keys;
        delete[] cursor->pointers;
        delete cursor;
        root = nullptr;
        return 1;
      }
      return 0;
    }

    // Shift the pointer to the adjacent leaf node.
    cursor->pointers[cursor->size] = cursor->pointers[cursor->size + 1];
    cursor->pointers[cursor->size + 1] = nullptr;
    cout << "Deleted " << x.key_value << " "
         << " from leaf node successfully\n";
    numNode--;

    // There are sufficient keys in node.
    if (cursor->size >= (MAX + 1) / 2)
    {
      cout << "Sufficient nodes!" << endl;
      // Update parent if necessary (When position of key in leaf node to be removed is 0).
      if (pos == 0)
      {
        cout << "Update parent!" << endl;
        updateParent(cursor, cursor->keys[0]);
      }
      return 0;
    }

    cout << "Underflow condition!" << endl;
    // Underflow condition.
    // Try to transfer a key from sibling node.
    // Check if left sibling exists.
    if (leftSiblingPtr >= 0)
    {
      cout << "Shift from left sibling!" << endl;
      Node *leftNode = parent->pointers[leftSiblingPtr];
      // Check if it is possible to transfer.
      if (leftNode->size >= (MAX + 1) / 2 + 1)
      {
        // Create space for transferring key.
        for (int i = cursor->size; i > 0; i--)
        {
          cursor->keys[i] = cursor->keys[i - 1];
        }
        // Update pointer to the next node.
        cursor->size++;
        cursor->pointers[cursor->size] = cursor->pointers[cursor->size - 1];
        cursor->pointers[cursor->size - 1] = nullptr;

        // Transfer key from left sibling to node.
        cursor->keys[0] = leftNode->keys[leftNode->size - 1];

        // Update pointer to the next node.
        leftNode->size--;
        leftNode->pointers[leftNode->size] = cursor;
        leftNode->pointers[leftNode->size + 1] = nullptr;

        // Update parent.
        parent->keys[leftSiblingPtr] = cursor->keys[0];
        return 0;
      }
    }
    // Check if right sibling exists.
    if (rightSiblingPtr <= parent->size)
    {
      cout << "Shift from right sibling!" << endl;
      Node *rightNode = parent->pointers[rightSiblingPtr];
      // Check if it is possible to transfer.
      if (rightNode->size >= (MAX + 1) / 2 + 1)
      {

        // Shift pointer that points to the next leaf.
        cursor->size++;
        cursor->pointers[cursor->size] = cursor->pointers[cursor->size - 1];
        cursor->pointers[cursor->size - 1] = nullptr;

        // Transfer key from right sibling.
        cursor->keys[cursor->size - 1] = rightNode->keys[0];

        // Update pointer of right sibling.
        rightNode->size--;
        rightNode->pointers[rightNode->size] = rightNode->pointers[rightNode->size + 1];
        rightNode->pointers[rightNode->size + 1] = nullptr;

        // Shift keys of right sibling down by 1.
        // No need to shift pointers cause they are null.
        for (int i = 0; i < rightNode->size; i++)
        {
          rightNode->keys[i] = rightNode->keys[i + 1];
        }
        // Update parent key.
        parent->keys[rightSiblingPtr - 1] = rightNode->keys[0];
        return 0;
      }
    }
    // If left sibling exists, merge with it.
    if (leftSiblingPtr >= 0)
    {
      cout << "Merged with left sibling!" << endl;
      Node *leftNode = parent->pointers[leftSiblingPtr];

      // Transfer all keys to left sibling.
      for (int i = leftNode->size, j = 0; j < cursor->size; i++, j++)
      {
        leftNode->keys[i] = cursor->keys[j];
      }
      leftNode->pointers[leftNode->size] = nullptr;
      leftNode->size += cursor->size;

      // Transfer pointer to adjacent leaf node.
      leftNode->pointers[leftNode->size] = cursor->pointers[cursor->size];

      // Delete parent node key of deleted cursor.
      int numNodeMerged = removeInternal(parent->keys[leftSiblingPtr], parent, cursor);

      delete[] cursor->keys;
      delete[] cursor->pointers;
      delete cursor;
      cout << "Deleted 1" << endl;
      return 1 + numNodeMerged;
    }
    // Else if right sibling exists, merge with it.
    else if (rightSiblingPtr <= parent->size)
    {
      cout << "Merged with right sibling!" << endl;
      Node *rightNode = parent->pointers[rightSiblingPtr];

      // Transfer all keys to cursor.
      for (int i = cursor->size, j = 0; j < rightNode->size; i++, j++)
      {
        cursor->keys[i] = rightNode->keys[j];
      }

      // Transfer pointer to adjacent leaf node.
      cursor->pointers[cursor->size] = nullptr;
      cursor->size += rightNode->size;
      cursor->pointers[cursor->size] = rightNode->pointers[rightNode->size];

      // Delete parent node key.
      int numNodesMerged = removeInternal(parent->keys[rightSiblingPtr - 1], parent, rightNode);
      delete[] rightNode->keys;
      delete[] rightNode->pointers;
      delete rightNode;
      cout << "Deleted 1 " << endl;
      return 1 + numNodesMerged;
    }
  }
}

/**
 * @brief 
 * If the first key of the leaf node is deleted, you have to update one of the index nodes whose key
 * that ultimately points to that leaf node.
 * However, if the smallest pointer of that index node leads to the leaf node, you have to continue
 * finding the appropriate parent.
 * @param child 
 * @param key 
 */
void BPTree::updateParent(Node *child, keys_struct key)
{
  Node *parent = findParent(root, child);
  int ptrPos = 0;
  while (ptrPos <= parent->size && parent->pointers[ptrPos] != child)
    ptrPos++;
  // If the first pointer is pointing to the child, find the previous parent.
  if (ptrPos == 0)
  {
    if (parent == root)
    {
      updateParent(parent, key);
    }
    else
    {
      return;
    }
  }
  else
  {
    parent->keys[ptrPos - 1] = key;
    return;
  }
}

/**
 * @brief
 * Use depth-first search to find parent of child.
 * @param cursor Possible parent.
 * @param child Child.
 * @return Node*
 */
Node *BPTree::findParent(Node *cursor, Node *child)
{
  Node *parent;

  // If cursor is on the last/second last level, skip search.
  // Cursor cannot be a parent if it is leaf node.
  // Cursor cannot be second-last level since findParent() is not called when
  // child node is leaf during insertion.
  if (cursor->isLeaf || (cursor->pointers[0])->isLeaf)
  {
    return NULL;
  }
  // Iterate through each pointer and check if pointer points to child.
  for (int i = 0; i < cursor->size + 1; i++)
  {
    if (cursor->pointers[i] == child)
    {
      parent = cursor;
      return parent;
    }
    // Else try to find parent from children of cursor.
    else
    {
      parent = findParent(cursor->pointers[i], child);
      if (parent != NULL)
        return parent;
    }
  }
  return parent;
}

/**
 * @brief
 * Search for leaf nodes within a specified range.
 * @param lowerBoundKey Lower range.
 * @param upperBoundKey Upper range.
 */
void BPTree::searchRange(float lowerBoundKey, float upperBoundKey)
{
  int numIndexNodes = 0;
  if (root == nullptr)
  {
    throw std::logic_error("Tree is empty!");
  }
  else
  {
    Node *cursor = root;
    // While leaf node is not reached yet.
    int level = 0;
    while (!cursor->isLeaf)
    {
      cout << "Index node at level: " << level << endl;
      for (int k = 0; k < cursor->size; k++)
      {
        cout << cursor->keys[k].key_value << "|";
      }
      cout << endl;
      numIndexNodes++;
      // Iterate through
      if (lowerBoundKey >= cursor->keys[cursor->size - 1].key_value)
      {
        cursor = cursor->pointers[cursor->size];
      }
      else
      {
        int i = 0;
        while (lowerBoundKey >= cursor->keys[i].key_value)
          i++;
        cursor = cursor->pointers[i];
      }
      level++;
    }
    cout << "Number of index nodes accessed: " << numIndexNodes << endl;
    // Here, we are at the leaf nodes. We then traverse through the nodes from
    // the lowerBoundKey to the upperBoundKey.
    int i = 0;
    while (cursor->keys[i].key_value < lowerBoundKey)
      i++;
    float totalRating = 0;
    int totalRecords = 0;
    set<uint> s;
    while (cursor->keys[i].key_value <= upperBoundKey)
    {
      for (int j = 0; j < cursor->keys[i].add_vect.size(); j++)
      {
        float averageRating =
            (*(Record *)cursor->keys[i].add_vect[j]).averageRating;
        totalRating += averageRating;
        intptr_t dataBlockNo = ((intptr_t)(Record *)cursor->keys[i].add_vect[j] -
                                (intptr_t)startAddress) /
                               blockSize;
        s.insert(dataBlockNo);
        totalRecords++;
        // get tconst value
        cout << (*(Record *)cursor->keys[i].add_vect[j]).tconst
             << " | "
             // get record address
             << (Record *)cursor->keys[i].add_vect[j]
             << " | "
             // get block no
             << dataBlockNo
             << " | "
             // get block addr
             << (Record *)((intptr_t)startAddress + (dataBlockNo * blockSize))
             << endl;
      }
      if (i == cursor->size - 1 && cursor->pointers[cursor->size] != nullptr &&
          cursor->keys[i].key_value <= upperBoundKey)
      {
        cursor = cursor->pointers[cursor->size];
        i = 0;
        continue;
      }
      i++;
    }
    cout << "Total number of records: " << totalRecords << endl;
    cout << "Average Rating: " << totalRating / totalRecords << endl;
    cout << "Number of data blocks accessed: " << s.size() << endl;
  }
}

/**
 * @brief
 * Search for a single leaf node with a specified key value.
 * @param key Key to be searched for.
 */
void BPTree::searchSingle(float key)
{
  int numIndexNodes = 0;
  if (root == nullptr)
  {
    throw std::logic_error("Tree is empty!");
  }
  else
  {
    Node *cursor = root;
    // While leaf node is not reached yet.
    int level = 0;
    while (!cursor->isLeaf)
    {
      cout << "Index node at level: " << level << endl;
      for (int k = 0; k < cursor->size; k++)
      {
        cout << cursor->keys[k].key_value << "|";
      }
      cout << endl;
      numIndexNodes++;
      // Iterate through
      if (key >= cursor->keys[cursor->size - 1].key_value)
      {
        cursor = cursor->pointers[cursor->size];
      }
      else
      {
        int i = 0;
        while (key >= cursor->keys[i].key_value)
          i++;
        cursor = cursor->pointers[i];
      }
      level++;
    }
    cout << "Number of index nodes accessed: " << numIndexNodes << endl;
    int i = 0;
    while (cursor->keys[i].key_value < key)
      i++;
    float totalRating = 0;
    set<uint> s;
    if (cursor->keys[i].key_value == key)
    {
      cout << "Number of records: " << cursor->keys[i].add_vect.size() << endl;
      for (int j = 0; j < cursor->keys[i].add_vect.size(); j++)
      {
        // cout << (*(Record *) cursor->key[i].add_vect[j]).tconst << "\n";
        totalRating += (*(Record *)cursor->keys[i].add_vect[j]).averageRating;
        intptr_t dataBlockNo = ((intptr_t)(Record *)cursor->keys[i].add_vect[j] -
                                (intptr_t)startAddress) /
                               blockSize;
        s.insert(dataBlockNo);
        // get tconst value
        cout << (*(Record *)cursor->keys[i].add_vect[j]).tconst
             << " | "
             // get record address
             << (Record *)cursor->keys[i].add_vect[j]
             << " | "
             // get block no
             << dataBlockNo
             << " | "
             // get block addr
             << (Record *)((intptr_t)startAddress + (dataBlockNo * blockSize))
             << endl;
      }
    }
    else
    {
      cout << "Does not exist!" << endl;
    }
    cout << "Average Rating: " << totalRating / cursor->keys[i].add_vect.size()
         << endl;
    cout << "Number of data blocks accessed: " << s.size() << endl;
  }
}

/**
 * @brief
 * Recursively traverse tree to find height.
 * @param cursor
 * @return int Height of tree.
 */
int BPTree::getHeight(Node *cursor)
{
  if (cursor->isLeaf)
  {
    return 1;
  }
  return getHeight(cursor->pointers[0]) + 1;
}

/**
 * @brief
 * Prints entire tree.
 * @param cursor Root node.
 */
void BPTree::printEntireTree(Node *cursor)
{
  // Base Case
  if (cursor == NULL)
    return;

  // Create an empty queue for level order traversal
  queue<Node *> q;

  // Enqueue root
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
        cout << node->keys[i].key_value << "|";
        if (!node->isLeaf)
        {
          q.push(node->pointers[i]);
        }
      }
      cout << "-----";
      if (!node->isLeaf)
      {
        q.push(node->pointers[node->size]);
      }
      q.pop();
    }
    cout << endl;
  }
}

/**
 * @brief
 * Display root node and 1st child node.
 * @param cursor
 */
void BPTree::display(Node *cursor)
{
  cout << "Root node: " << endl;
  for (int i = 0; i < cursor->size; i++)
  {
    cout << cursor->keys[i].key_value << "|";
  }
  cout << endl;
  cout << "First child node: " << endl;
  cursor = cursor->pointers[0];
  for (int i = 0; i < cursor->size; i++)
  {
    cout << cursor->keys[i].key_value << "|";
  }
  cout << endl;
}

/**
 * @brief
 * Calculate the number of nodes with BFS.
 * @param cursor Root node.
 * @return int number of nodes in tree.
 */
int BPTree::calculateNumNodes(Node *cursor)
{
  // Base Case
  if (cursor == NULL)
    return 0;

  // Create an empty queue for level order traversal
  queue<Node *> q;

  // Enqueue root
  q.push(cursor);

  int numNodes = 0;

  while (!q.empty())
  {
    int queueSize = q.size();
    numNodes += queueSize;

    for (int i = 0; i < queueSize; i++)
    {
      // Print front of queue and remove it from queue
      Node *node = q.front();
      for (int i = 0; i < node->size; i++)
      {
        if (!node->isLeaf)
        {
          q.push(node->pointers[i]);
        }
      }
      if (!node->isLeaf)
      {
        q.push(node->pointers[node->size]);
      }
      q.pop();
    }
  }
  return numNodes;
}

int BPTree::getMax() { return MAX; }

Node *BPTree::getRoot() { return root; }

BPTree::BPTree() : root{NULL} {}
