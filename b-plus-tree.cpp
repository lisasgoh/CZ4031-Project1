// int MAX; //size of each node
#include "b-plus-tree.h"
using namespace std;
extern void *startAddress;

Node::Node() {
  // dynamic memory allocation
  key = new keys_struct[MAX];
  ptr = new Node *[MAX + 1];
  IS_LEAF = true;
}

void BPTree::insertInternal(keys_struct x, Node *cursor, Node *child) {
  // insert into current node if there is space in cursor
  if (cursor->size < MAX) {
    // since cursor is not full, linear search for position of new key
    int newKeyPos = 0; // position of new key
    while (x.key_value > cursor->key[newKeyPos].key_value &&
           newKeyPos < cursor->size) {
      newKeyPos++;
    }
    // shift keys above new key up
    for (int i = cursor->size; i > newKeyPos; i--) {
      cursor->key[i] =
          cursor->key[i -
                      1]; // push each key above the position of inserted key up
    }
    // shift pointers above new pointer up
    for (int i = cursor->size + 1; i > newKeyPos + 1; i--) {
      cursor->ptr[i] = cursor->ptr[i - 1];
    }
    cursor->key[newKeyPos] = x;         // set new key
    cursor->size++;                     // set new size
    cursor->ptr[newKeyPos + 1] = child; // set new pointer
    // cout<<"Inserted key in an Internal node successfully\n";
  }
  // split internal node if overflow
  else {
    // cout<<"Inserted key in an Internal node successfully\n";
    // cout<<"Overflow in internal node!\nSplitting internal node\n";

    // create another internal node when overflow
    Node *newInternal = new Node;
    numNode++;
    newInternal->IS_LEAF = false;

    // create temporary virtual Node to be split
    // node with 1 extra key and pointer
    keys_struct virtualKey[MAX + 1];
    Node *virtualPtr[MAX + 2];

    // duplicate cursor node keys and pointers to virtual node
    for (int i = 0; i < MAX; i++) {
      virtualKey[i] = cursor->key[i];
    }
    for (int i = 0; i < MAX + 1; i++) {
      virtualPtr[i] = cursor->ptr[i];
    }
    int j;
    int newKeyPos = 0; // position of new key

    // since cursor is not full, linear search for position of new key
    while (x.key_value > virtualKey[newKeyPos].key_value && newKeyPos < MAX) {
      newKeyPos++;
    }

    // shift keys above new key up
    for (int j = MAX + 1; j > newKeyPos; j--) {
      virtualKey[j] = virtualKey[j - 1];
    }
    virtualKey[newKeyPos] = x; // set new key

    // shift pointers above new pointer up
    for (int j = MAX + 2; j > newKeyPos + 1; j--) {
      virtualPtr[j] = virtualPtr[j - 1];
    }

    virtualPtr[newKeyPos + 1] = child; // set new pointer

    // split cursor to cursor and newInternal with half of original size
    cursor->size = (MAX + 1) / 2;
    newInternal->size = MAX - (MAX + 1) / 2;

    // set keys and pointers of new node
    int i;
    for (i = 0, j = cursor->size + 1; i < newInternal->size; i++, j++) {
      newInternal->key[i] = virtualKey[j];
    }
    for (i = 0, j = cursor->size + 1; i < newInternal->size + 1; i++, j++) {
      newInternal->ptr[i] = virtualPtr[j];
    }
    // the note we are splitting is a root node
    if (root == cursor) {
      // creation of new node when splitting root node
      Node *newRoot = new Node;
      numNode++;
      newRoot->key[0] = cursor->key[cursor->size];
      newRoot->ptr[0] = cursor;
      newRoot->ptr[1] = newInternal;
      newRoot->IS_LEAF = false;
      newRoot->size = 1;
      root = newRoot;
      // cout<<"Created new root\n";
    } else { // recursive DFS to find parent node
      insertInternal(cursor->key[cursor->size], findParent(root, cursor),
                     newInternal);
    }
  }
}

int BPTree::removeInternal(keys_struct x, Node *cursor, Node *child) {
  // deleting the key x first
  // if deleting root node key
  if (cursor == root) {
    // if root empty after removal, child becomes new root
    if (cursor->size == 1) {
      // check if larger or smaller pointer points to child
      // other pointer that do not point to child becomes new root
      int newRoot;
      // check if 1 of 2 nodes points to child
      if (cursor->ptr[0] == child || cursor->ptr[1] == child) {
        // check if larger pointer points to child
        if (cursor->ptr[1] == child) {
          // set new root to other child
          newRoot = 0;
        } else {
          // set new root to other child
          newRoot = 1;
        }
        // delete child
        delete[] child->key;
        delete[] child->ptr;
        delete child;
        numNode--;
        cout << "Deleted 1"
             << "\n";
        // set new root
        root = cursor->ptr[newRoot];
        // delete old root node
        delete[] cursor->key;
        delete[] cursor->ptr;
        delete cursor;
        numNode--;
        cout << "Deleted 1"
             << "\n";
        // cout<<"Changed root node\n";
        return 0;
      }

      //            if(cursor->ptr[1] == child)
      //            {
      //                delete[] child->key;
      //                delete[] child->ptr;
      //                delete child;
      //                cout << "Deleted 1" <<"\n";
      //                root = cursor->ptr[0];
      //                delete[] cursor->key;
      //                delete[] cursor->ptr;
      //                delete cursor;
      //                cout << "Deleted 1" <<"\n";
      //                //cout<<"Changed root node\n";
      //                return;
      //            }
      //            else if(cursor->ptr[0] == child)
      //            {
      //                delete[] child->key;
      //                delete[] child->ptr;
      //                delete child;
      //                cout << "Deleted 1" <<"\n";
      //                root = cursor->ptr[1];
      //                delete[] cursor->key;
      //                delete[] cursor->ptr;
      //                delete cursor;
      //                cout << "Deleted 1" <<"\n";
      //                //cout<<"Changed root node\n";
      //                return;
      //            }
    }
  }
  // when reach here, parent is not root node with 1 key
  int pos;
  // search for position of key to be deleted
  for (pos = 0; pos < cursor->size; pos++) {
    // position acquired
    if (cursor->key[pos].key_value == x.key_value) {
      break;
    }
  }
  // shift each key from pos above back, removing the key
  for (int i = pos; i < cursor->size; i++) {
    cursor->key[i] = cursor->key[i + 1];
  }
  // search for position of pointer to be deleted
  for (pos = 0; pos < cursor->size + 1; pos++) {
    // position acquired
    if (cursor->ptr[pos] == child) {
      break;
    }
  }
  // shift each pointer from pos above back, removing the pointer
  for (int i = pos; i < cursor->size + 1; i++) {
    cursor->ptr[i] = cursor->ptr[i + 1];
  }
  // update new size
  cursor->size--;
  // check if size satisfy minimum size
  if (cursor->size >= (MAX + 1) / 2 - 1) {
    cout << "Deleted " << x.key_value << " "
         << " from internal node successfully\n";
    return 0;
  }
  cout << "Underflow in internal node!\n";
  // when reach here, size is too small(underflow)

  // ignore minimum size if node is root
  if (cursor == root) {
    return 0;
  }
  // strategy: try to get keys from sibling nodes
  // locate parent to find siblings
  Node *parent = findParent(root, cursor);
  int leftSibling, rightSibling;
  // get position of node in parent
  for (pos = 0; pos < parent->size + 1; pos++) {
    if (parent->ptr[pos] == cursor) {
      // locate the nearest siblings
      leftSibling = pos - 1;
      rightSibling = pos + 1;
      break;
    }
  }
  // get keys from siblings
  // check existence of left sibling
  if (leftSibling >= 0) {
    // get left sibling
    Node *leftNode = parent->ptr[leftSibling];
    // check if left sibling can afford to transfer (hit minimum size)
    if (leftNode->size >= (MAX + 1) / 2) {

      // make key space for incoming transfer
      for (int i = cursor->size; i > 0; i--) {
        // shift keys to the right since incoming key is smallest
        cursor->key[i] = cursor->key[i - 1];
      }
      // move right most key of left sibling to cursor left most key
      cursor->key[0] = parent->key[leftSibling];
      parent->key[leftSibling] = leftNode->key[leftNode->size - 1];

      // make pointer space for incoming transfer
      for (int i = cursor->size + 1; i > 0; i--) {
        // shift pointers to the right since incoming pointer is smallest
        cursor->ptr[i] = cursor->ptr[i - 1];
      }
      // move right most pointer of left sibling to cursor left most pointer
      cursor->ptr[0] = leftNode->ptr[leftNode->size];
      // update new node sizes
      cursor->size++;
      leftNode->size--;
      cout << "Transferred " << cursor->key[0].key_value << " " << x.add_vect[0]
           << " from left sibling of internal node\n";
      return 0;
    }
  }
  // check existence of right sibling
  if (rightSibling <= parent->size) {
    // get right sibling
    Node *rightNode = parent->ptr[rightSibling];
    // check if right sibling can afford to transfer (hit minimum size)
    if (rightNode->size >= (MAX + 1) / 2) {
      // do not need to make space since transferred key/pointer is largest in
      // cursor move left most key of right sibling to cursor right most key
      cursor->key[cursor->size] = parent->key[pos];
      // update parent key for right sibling
      parent->key[pos] = rightNode->key[0];
      // update right sibling keys by shifting left
      for (int i = 0; i < rightNode->size - 1; i++) {
        rightNode->key[i] = rightNode->key[i + 1];
      }
      // move right most pointer of left sibling to cursor left most pointer
      cursor->ptr[cursor->size + 1] = rightNode->ptr[0];
      // update right sibling pointers by shifting left
      for (int i = 0; i < rightNode->size; ++i) {
        rightNode->ptr[i] = rightNode->ptr[i + 1];
      }
      // update new node sizes
      cursor->size++;
      rightNode->size--;
      cout << "Transferred " << cursor->key[0].key_value << " " << x.add_vect[0]
           << " from right sibling of internal node\n";
      return 0;
    }
  }
  // if we reach here, it means that transferal from sibling nodes was not
  // possible hence we do merging check existence of left sibling
  if (leftSibling >= 0) {
    // merged node = left sibling + parent key + cursor
    // get left sibling
    Node *leftNode = parent->ptr[leftSibling];
    // left sibling becomes the merged node
    // left sibling = left sibling + parent key
    leftNode->key[leftNode->size] = parent->key[leftSibling];
    // add each cursor key into left sibling
    for (int i = leftNode->size + 1, j = 0; j < cursor->size; j++) {
      leftNode->key[i] = cursor->key[j];
    }
    // add each cursor pointer into left sibling
    for (int i = leftNode->size + 1, j = 0; j < cursor->size + 1; j++) {
      leftNode->ptr[i] = cursor->ptr[j];
      cursor->ptr[j] = NULL;
    }
    // update merged node size
    leftNode->size += cursor->size + 1;
    // update cursor size
    cursor->size = 0;
    // delete cursor
    int numNodeMerged = removeInternal(parent->key[leftSibling], parent, cursor);
    cout << "Merged with left sibling\n";
    return 1 + numNodeMerged;
  }
  // since can't merge with left, merge right
  // check existence of right sibling
  else if (rightSibling <= parent->size) {
    // merged node = cursor + parent key + right sibling
    Node *rightNode = parent->ptr[rightSibling];
    // cursor becomes the merged node
    // cursor = cursor + parent key
    cursor->key[cursor->size] = parent->key[rightSibling - 1];
    // add each right sibling key into cursor
    for (int i = cursor->size + 1, j = 0; j < rightNode->size; j++) {
      cursor->key[i] = rightNode->key[j];
    }
    // add each right sibling pointer into cursor
    for (int i = cursor->size + 1, j = 0; j < rightNode->size + 1; j++) {
      cursor->ptr[i] = rightNode->ptr[j];
      rightNode->ptr[j] = NULL;
    }
    // update merged node size
    cursor->size += rightNode->size + 1;
    // update right sibling size
    rightNode->size = 0;
    // delete right sibling
    int numNodeMerged = removeInternal(parent->key[rightSibling - 1], parent, rightNode);
    cout << "Merged with right sibling\n";
    return 1 + numNodeMerged;
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

BPTree::BPTree() : root{NULL} {}

Node *BPTree::search(float x, bool flag, bool printer) {
  // search logic
  //    if(root==NULL)
  //    {
  //        //empty
  //        //cout<<"Tree empty\n";
  //    }
  int nodesAccessed = 0;
  if (printer) {
    cout << "Nodes traversed:" << endl;
  }
  if (root != NULL) {
    Node *cursor = root;
    // in the following while loop, cursor will travel to the leaf node possibly
    // consisting the key
    while (cursor->IS_LEAF == false) {
      // traverse each key in cursor
      for (int i = 0; i < cursor->size; i++) {
        // if key value larger than target, left pointer contains key if key is
        // in cursor
        if (x < cursor->key[i].key_value) {
          nodesAccessed++;
          // print each cursor key
          if (printer == true) {
            for (int j = 0; j < cursor->size; j++) {
              cout << cursor->key[j].key_value << " ";
            }

            cout << "\n";
          }
          // set new cursor as left pointer
          cursor = cursor->ptr[i];
          break;
        }
        // if traversed till end of cursor, target must be in right most pointer
        if (i == cursor->size - 1) {
          nodesAccessed++;
          if (printer == true) {
            for (int j = 0; j < cursor->size; j++) {
              cout << cursor->key[j].key_value << " ";
            }
            cout << "\n";
          }
          // set new cursor as right pointer
          cursor = cursor->ptr[i + 1];
          break;
        }
      }
    }
    // we reach here once the leaf node that could contain target is reached
    if (printer == true) {

      for (int j = 0; j < cursor->size; j++) {
        cout << cursor->key[j].key_value << " ";
      }
      cout << "\n";
    }
    // linear search leaf keys for target
    for (int i = 0; i < cursor->size; i++) {
      // target found
      if (cursor->key[i].key_value == x) {
        // cout<<"Found\n";
        if (flag == true) {
          // accessed leaf node
          nodesAccessed++;
          cout << "nodes accessed: " << nodesAccessed << endl;
          cout << "Size: " << cursor->key[i].add_vect.size() << "\n";
          printf("Number of Data Blocks: %d", cursor->key[i].add_vect.size());
          printf("\n");

          float totalRating = 0;
          for (int j = 0; j < cursor->key[i].add_vect.size(); ++j) {
            printf("Data Block: ");
            cout << (Record *)cursor->key[i].add_vect[j] << endl;
            printf("\n");
            printf("tconst: ");
            cout << (*(Record *)cursor->key[i].add_vect[j]).tconst << "\n";
            totalRating +=
                (*(Record *)cursor->key[i].add_vect[j]).averageRating;
          }
          cout << "Average Rating: "
               << totalRating / cursor->key[i].add_vect.size() << endl;
          cout << "start address:" << startAddress << endl;
        }
        return cursor;
      }
    }
    // if reach here, target is not found
    // cout<<"Not found\n";
    return nullptr;
  }
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

void BPTree::insert(keys_struct x) {
  // insert logic
  if (root == NULL) {
    root = new Node;
    numNode++;
    root->key[0] = x;
    root->IS_LEAF = true;
    root->size = 1;
    // cout<<"Created root\nInserted "<<  x.key_value << " " << x.add_vect[0]
    // <<" successfully\n";
  } else {
    Node *cursor = root;
    Node *parent;

    Node *searchRes;
    searchRes = search(x.key_value, false, false);
    if (searchRes != nullptr) {
      for (int i = 0; i < searchRes->size; i++) {
        if (searchRes->key[i].key_value == x.key_value) {
          searchRes->key[i].add_vect.push_back(x.add_vect[0]);
          break;
        }
      }
      return;
    }
    // in the following while loop, cursor will travel to the leaf node possibly
    // consisting the key
    while (cursor->IS_LEAF == false) {
      parent = cursor;
      for (int i = 0; i < cursor->size; i++) {
        if (x.key_value < cursor->key[i].key_value) {
          cursor = cursor->ptr[i];
          break;
        }
        if (i == cursor->size - 1) {
          cursor = cursor->ptr[i + 1];
          break;
        }
      }
    }
    // now cursor is the leaf node in which we'll insert the new key
    if (cursor->size < MAX) {
      // if cursor is not full
      // find the correct position for new key
      int i = 0;
      while (x.key_value > cursor->key[i].key_value && i < cursor->size)
        i++;
      // make space for new key
      for (int j = cursor->size; j > i; j--) {
        cursor->key[j] = cursor->key[j - 1];
      }
      cursor->key[i] = x;
      cursor->size++;
      cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
      cursor->ptr[cursor->size - 1] = NULL;
      // cout<<"Inserted "<< x.key_value << " " << x.add_vect[0] <<"
      // successfully\n";
    } else {
      // cout<<"Inserted "<< x.key_value << " " << x.add_vect[0] <<"
      // successfully\n"; cout<<"Overflow in leaf node!\nSplitting leaf node\n";
      // overflow condition
      // create new leaf node
      Node *newLeaf = new Node;
      numNode++;
      // create a virtual node and insert x into it
      keys_struct virtualNode[MAX + 1];
      for (int i = 0; i < MAX; i++) {
        virtualNode[i] = cursor->key[i];
      }
      int i = 0, j;
      while (x.key_value > virtualNode[i].key_value && i < MAX)
        i++;
      // make space for new key
      for (int j = MAX + 1; j > i; j--) {
        virtualNode[j] = virtualNode[j - 1];
      }
      virtualNode[i] = x;
      newLeaf->IS_LEAF = true;
      // split the cursor into two leaf nodes
      cursor->size = (MAX + 1) / 2;
      newLeaf->size = MAX + 1 - (MAX + 1) / 2;
      // make cursor point to new leaf node
      cursor->ptr[cursor->size] = newLeaf;
      // make new leaf node point to the next leaf node
      newLeaf->ptr[newLeaf->size] = cursor->ptr[MAX];
      cursor->ptr[MAX] = NULL;
      // now give elements to new leaf nodes
      for (i = 0; i < cursor->size; i++) {
        cursor->key[i] = virtualNode[i];
      }
      for (i = 0, j = cursor->size; i < newLeaf->size; i++, j++) {
        newLeaf->key[i] = virtualNode[j];
      }
      // modify the parent
      if (cursor == root) {
        // if cursor is a root node, we create a new root
        Node *newRoot = new Node;
        numNode++;
        newRoot->key[0] = newLeaf->key[0];
        newRoot->ptr[0] = cursor;
        newRoot->ptr[1] = newLeaf;
        newRoot->IS_LEAF = false;
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

int BPTree::remove(keys_struct x) {
  // delete logic
  if (root == NULL) {
    cout << "Tree empty\n";
  } else {
    Node *cursor = root;
    Node *parent;
    int leftSibling, rightSibling;
    // in the following while loop, cursor will will travel to the leaf node
    // possibly consisting the key
    while (cursor->IS_LEAF == false) {
      for (int i = 0; i < cursor->size; i++) {
        parent = cursor;
        leftSibling =
            i -
            1; // leftSibling is the index of left sibling in the parent node
        rightSibling =
            i +
            1; // rightSibling is the index of right sibling in the parent node
        if (x.key_value < cursor->key[i].key_value) {
          cursor = cursor->ptr[i];
          break;
        }
        if (i == cursor->size - 1) {
          leftSibling = i;
          rightSibling = i + 2;
          cursor = cursor->ptr[i + 1];
          break;
        }
      }
    }
    // in the following for loop, we search for the key if it exists
    bool found = false;
    int pos;
    for (pos = 0; pos < cursor->size; pos++) {
      if (cursor->key[pos].key_value == x.key_value) {
        found = true;
        break;
      }
    }
    if (!found) // if key does not exist in that leaf node
    {
      cout << "Not found\n";
      return 0;
    }
    // deleting the key
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
        cout << "Tree died\n";
        delete[] cursor->key;
        delete[] cursor->ptr;
        delete cursor;
        numNode--;
        cout << "Deleted 1"
             << "\n";
        root = NULL;
      }
      return 0;
    }
    cursor->ptr[cursor->size] = cursor->ptr[cursor->size + 1];
    cursor->ptr[cursor->size + 1] = NULL;
    cout << "Deleted " << x.key_value << " "
         << " from leaf node successfully\n";
    if (cursor->size >= (MAX + 1) / 2) // no underflow
    {
      return 0;
    }
    cout << "Underflow in leaf node!\n";
    // underflow condition
    // first we try to transfer a key from sibling node
    // check if left sibling exists
    if (leftSibling >= 0) {
      Node *leftNode = parent->ptr[leftSibling];
      // check if it is possible to transfer
      if (leftNode->size >= (MAX + 1) / 2 + 1) {
        // make space for transfer
        for (int i = cursor->size; i > 0; i--) {
          cursor->key[i] = cursor->key[i - 1];
        }
        // shift pointer to next leaf
        cursor->size++;
        cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
        cursor->ptr[cursor->size - 1] = NULL;
        // transfer
        cursor->key[0] = leftNode->key[leftNode->size - 1];
        // shift pointer of leftsibling
        leftNode->size--;
        leftNode->ptr[leftNode->size] = cursor;
        leftNode->ptr[leftNode->size + 1] = NULL;
        // update parent
        parent->key[leftSibling] = cursor->key[0];
        cout << "Transferred " << cursor->key[0].key_value << " "
             << cursor->key[0].add_vect[0]
             << " from left sibling of leaf node\n";
        return 0;
      }
    }
    if (rightSibling <= parent->size) // check if right sibling exist
    {
      Node *rightNode = parent->ptr[rightSibling];
      // check if it is possible to transfer
      if (rightNode->size >= (MAX + 1) / 2 + 1) {
        // shift pointer to next leaf
        cursor->size++;
        cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
        cursor->ptr[cursor->size - 1] = NULL;
        // transfer
        cursor->key[cursor->size - 1] = rightNode->key[0];
        // shift pointer of rightsibling
        rightNode->size--;
        rightNode->ptr[rightNode->size] = rightNode->ptr[rightNode->size + 1];
        rightNode->ptr[rightNode->size + 1] = NULL;
        // shift conent of right sibling
        for (int i = 0; i < rightNode->size; i++) {
          rightNode->key[i] = rightNode->key[i + 1];
        }
        // update parent
        parent->key[rightSibling - 1] = rightNode->key[0];
        cout << "Transferred " << cursor->key[cursor->size - 1].key_value << " "
             << cursor->key[cursor->size - 1].add_vect[0]
             << " from right sibling of leaf node\n";
        return 0;
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

      cout << "Merging two leaf nodes\n";
      int numNodeMerged = removeInternal(parent->key[leftSibling], parent,
                     cursor); // delete parent node key
      delete[] cursor->key;
      delete[] cursor->ptr;
      delete cursor;
      numNode--;
      cout << "Deleted 1"
           << "\n";
      return 1 + numNodeMerged;
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
      cout << "Merging two leaf nodes\n";
      int numNodeMerged  =removeInternal(parent->key[rightSibling - 1], parent,
                     rightNode); // delete parent node key
      delete[] rightNode->key;
      delete[] rightNode->ptr;
      delete rightNode;
      numNode--;
      cout << "Deleted 1 \n";
      return 1 + numNodeMerged;
    }
  }
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
    if (cursor->IS_LEAF == false) {
      for (int i = 0; i < cursor->size + 1; i++) {
        thisnode = display(cursor->ptr[i], 0, false);
        nodecount = nodecount + thisnode;
        // cout<<"\n"<<nodecount<<"\n";

        // read(curr->child_ptr[i]);
      }
      nodecount++;
      return nodecount;
    }
    if (cursor->IS_LEAF == true) {

      for (int i = 0; i < cursor->size; i++) {
        cout << cursor->key[i].key_value << " ";
      }
      return 1;
    }
    return 0;
  }
}

int BPTree::getMax() { return MAX; }

Node *BPTree::getRoot() { return root; }
