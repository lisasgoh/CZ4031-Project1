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
    key = new keys_struct[MAX];
    ptr = new Node *[MAX + 1];
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
        while (i < cursor->size && x.key_value > cursor->key[i].key_value)
            i++;

        // Make space for new key.
        for (int j = cursor->size; j > i; j--)
        {
            cursor->key[j] = cursor->key[j - 1];
            cursor->ptr[j + 1] = cursor->ptr[j];
        }
        cursor->key[i] = x;         // Set new key.
        cursor->size++;             // Set new size.
        cursor->ptr[i + 1] = child; // Set new pointer.
    }
    else
    {
        // create another internal node when overflow
        Node *newInternal = new Node;
        numNode++;
        newInternal->isLeaf = false;

        // Create temporary node with extra key and pointer to store keys and pointers.
        keys_struct tempKeys[MAX + 1];

        // MAX refers to the number of keys, pointer = number of keys + 1
        Node *tempPointers[MAX + 2];

        // Copy keys and nodes to the temporary node.
        for (int i = 0; i < MAX; i++)
        {
            tempKeys[i] = cursor->key[i];
            tempPointers[i] = cursor->ptr[i];
        }
        tempPointers[MAX] = cursor->ptr[MAX];

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
            cursor->key[i] = tempKeys[i];
            cursor->ptr[i] = tempPointers[i];
        }
        cursor->ptr[cursor->size] = tempPointers[i];

        // Set keys and pointers of new node.
        // Skip the first key of the new internal node.
        for (i = 0, j = cursor->size + 1; i < newInternal->size; i++, j++)
        {
            newInternal->key[i] = tempKeys[j];
            newInternal->ptr[i] = tempPointers[j];
        }
        newInternal->ptr[newInternal->size] = tempPointers[j];

        if (root == cursor)
        {
            Node *newRoot = new Node;
            numNode++;
            // New key that should be added in the root node.
            keys_struct newKey = getNewKey(newInternal);
            newRoot->key[0] = newKey;
            newRoot->ptr[0] = cursor;
            newRoot->ptr[1] = newInternal;
            newRoot->isLeaf = false;
            newRoot->size = 1;
            root = newRoot;
            cout << "Created new root!" << endl;
        }
        else
        {   // New key that should be added in the parent node.
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
        cursor = cursor->ptr[0];
    }
    return cursor->key[0];
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
            parent = cursor;
            if (x.key_value >= cursor->key[cursor->size - 1].key_value)
            {
                cursor = cursor->ptr[cursor->size];
            }
            else
            {
                // // Iterate through the keys to find the relevant key/ptr
                int i = 0;
                while (x.key_value >= cursor->key[i].key_value)
                    i++;
                cursor = cursor->ptr[i];
            }
        }
        // Cursor is now the leaf node in which the new key will be inserted.
        // If there is space to insert the new key.
        for (int k = 0; k < cursor->size; k++)
        {
            if (cursor->key[k].key_value == x.key_value)
            {
                cursor->key[k].add_vect.push_back(x.add_vect[0]);
                return;
            }
        }
        // There is space for new element.
        if (cursor->size < MAX)
        {
            int i = 0;

            // Continue iterating while cursor has not reached the last key and the key to be inserted is larger
            // than the current key.
            if (x.key_value > cursor->key[cursor->size - 1].key_value)
            {
                i = cursor->size;
            }
            while (i < cursor->size && x.key_value > cursor->key[i].key_value)
            {
                i++;
            }
            // Make space for new key
            for (int j = cursor->size; j > i; j--)
            {
                cursor->key[j] = cursor->key[j - 1];
            }

            // Insert key into the node.
            cursor->key[i] = x;
            cursor->size++;
            cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
            cursor->ptr[cursor->size - 1] = NULL;
            return;
        }
        //Overflow, due to insufficient space to insert new key.
        else
        {
            // cout <<  "MORE THAN MAX!" <<endl;
            Node *newLeaf = new Node;
            newLeaf->isLeaf = true;

            // Create a temp node to accommodate all the keys and insert x into it
            keys_struct tempNode[MAX + 1];
            for (int i = 0; i < MAX; i++)
            {
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
            cursor->ptr[cursor->size] = newLeaf;

            // Set the rightmost pointer of the new leaf node to the next leaf node.
            newLeaf->ptr[newLeaf->size] = cursor->ptr[MAX];
            cursor->ptr[MAX] = nullptr;

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

                // Set the new root's key to be the left bound of the right child.
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
                // Insert first key of new leaf intp parent node.
                insertInternal(newLeaf->key[0], parent, newLeaf);
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
 * @param newNode The retained node.
 * @return int number of nodes that are merged.
 */
int BPTree::removeInternal(keys_struct x, Node *cursor, Node *child, Node *newNode)
{
    cout << "Remove internal node: " << x.key_value << endl;
    // If root node key is being deleted and root is empty after removal, child becomes new root.
    if (cursor == root && cursor->size == 1)
    {
        // check if larger or smaller pointer points to child
        // other pointer that do not point to child becomes new root
        // Check if the adjacent pointers of the first key points to child.
        if (cursor->ptr[0] == child || cursor->ptr[1] == child)
        {
            root = cursor->ptr[0] == child ? cursor->ptr[1] : cursor->ptr[0];

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
            return 0;
        }
    }
    // Search for the key that is to be deleted.
    int pos = 0;
    while (pos < cursor->size && x.key_value != cursor->key[pos].key_value)
        pos++;

    // Shift each key from pos above back, removing the key
    for (int i = pos; i < cursor->size; i++)
    {
        cursor->key[i] = cursor->key[i + 1];
    }
    // Search for the position of the pointer that is to be deleted.
    pos = 0;
    while (pos < cursor->size + 1 && cursor->ptr[pos] != child)
        pos++;

    // Shift each pointer to the left to remove pointer.
    for (int i = pos; i < cursor->size + 1; i++)
    {
        cursor->ptr[i] = cursor->ptr[i + 1];
    }
    cursor->ptr[pos - 1] = newNode; //TODO CHECK THIS (ADDED)
    // Update size of parent after removing key.
    cursor->size--;

    // Verify if new size satisfies minimum size.
    // Minimum size is irrelevant if node is root.
    if (cursor->size >= (MAX + 1) / 2 - 1 || cursor == root)
    {
        cout << "Deleted " << x.key_value << " "
             << " from internal node successfully\n";
        return 0;
    }
    // Try to get transfer keys from sibling nodes.

    // Get parent of internal node.
    Node *parent = findParent(root, cursor);

    int leftSibling, rightSibling;

    // Get index of pointer to the node in parent node.
    pos = 0;
    while (pos < parent->size + 1 && parent->ptr[pos] != cursor)
        pos++;
    leftSibling = pos - 1;
    rightSibling = pos + 1;

    // If left sibling exists, transfer key from left sibling.
    if (leftSibling >= 0)
    {
        cout << "Merge with left sibling!" << endl;
        Node *leftNode = parent->ptr[leftSibling];

        // Can transfer key from left sibling.
        if (leftNode->size >= (MAX + 1) / 2)
        {
            // Create space for key transfer to current node.
            for (int i = cursor->size; i > 0; i--)
            {
                // Shift all keys to the right.
                cursor->key[i] = cursor->key[i - 1];
            }

            // Transfer key from left sibling.
            cursor->key[0] = parent->key[leftSibling];
            parent->key[leftSibling] = leftNode->key[leftNode->size - 1];

            // Create space for pointer transfer.
            for (int i = cursor->size + 1; i > 0; i--)
            {
                // Shift all pointers to the right.
                cursor->ptr[i] = cursor->ptr[i - 1];
            }

            // Transfer pointer from left sibling.
            cursor->ptr[0] = leftNode->ptr[leftNode->size];

            // Update sizes of nodes.
            cursor->size++;
            leftNode->size--;
            cout << "Transferred " << cursor->key[0].key_value << "from left sibling of node!" << endl;
            return 0;
        }
    }
    // If right sibling exists, try to transfer from right sibling.
    if (rightSibling <= parent->size)
    {
        cout << "Merge with right sibling!" << endl;
        Node *rightNode = parent->ptr[rightSibling];

        // Can transfer key from right node.
        if (rightNode->size >= (MAX + 1) / 2)
        {
            // Do not need to make space since key to be transferred is largest in cursor.
            // pos represents the index of the pointer that points to the cursor node.
            // Shift leftmost key of right sibling to cursor's last key.
            cursor->key[cursor->size] = parent->key[pos];

            // Shift right sibling keys to the left.
            for (int i = 0; i < rightNode->size - 1; i++)
            {
                rightNode->key[i] = rightNode->key[i + 1];
            }

            // Move first pointer of right sibling to cursor's last pointer.
            cursor->ptr[cursor->size + 1] = rightNode->ptr[0];

            // Shift right sibling pointers to the left.
            for (int i = 0; i < rightNode->size; ++i)
            {
                rightNode->ptr[i] = rightNode->ptr[i + 1];
            }
            // Update parent key for right sibling.
            parent->key[pos] = rightNode->key[0];

            // Update sizes of nodes.
            cursor->size++;
            rightNode->size--;
            cout << "Transferred  " << cursor->key[0].key_value << " from right sibling of node!" << endl;
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
        for (int i = leftNode->size + 1, j = 0; j < cursor->size; j++)
        {
            leftNode->key[i] = cursor->key[j];
        }
        // Add each pointer from cursor into left sibling.
        for (int i = leftNode->size + 1, j = 0; j < cursor->size + 1; j++)
        {
            leftNode->ptr[i] = cursor->ptr[j];
            cursor->ptr[j] = nullptr;
        }
        // Update sizes.
        leftNode->size += cursor->size + 1;
        cursor->size = 0;

        // Passes the key of the cursor (Key to be removed).
        int numNodesMerged = removeInternal(parent->key[leftSibling], parent, cursor, leftNode);
        return 1 + numNodesMerged;
    }
    // Try to merge right sibling.
    else if (rightSibling <= parent->size)
    {
        cout << "Merged with right sibling!" << endl;

        // Merge right sibling into cursor.
        Node *rightNode = parent->ptr[rightSibling];
        cursor->key[cursor->size] = parent->key[rightSibling - 1];

        // Add each right sibling key into cursor
        for (int i = cursor->size + 1, j = 0; j < rightNode->size; j++)
        {
            cursor->key[i] = rightNode->key[j];
        }
        // Add each right sibling pointer into cursor
        for (int i = cursor->size + 1, j = 0; j < rightNode->size + 1; j++)
        {
            cursor->ptr[i] = rightNode->ptr[j];
            rightNode->ptr[j] = NULL;
        }
        // Update sizes.
        cursor->size += rightNode->size + 1;
        rightNode->size = 0;

        // Passes the key of the right sibling (Key to be removed).
        int numNodesMerged = removeInternal(parent->key[rightSibling - 1], parent, rightNode, cursor);
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
    // delete logic
    if (root == nullptr)
    {
        throw std::logic_error("Tree is empty!");
    }
    else
    {
        Node *cursor = root;
        Node *parent;

        int leftSibling, rightSibling;

        // Cursor travels to the leaf nodes.
        while (!cursor->isLeaf)
        {
            parent = cursor;
            int i;
            if (x.key_value >= cursor->key[cursor->size - 1].key_value)
            {
                i = cursor->size;
            }
            else
            {
                i = 0;
                while (x.key_value >= cursor->key[i].key_value)
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
        int pos = 0;
        while (x.key_value > cursor->key[pos].key_value)
            pos++;
        if (x.key_value != cursor->key[pos].key_value) // if key does not exist in that leaf node
        {
            cout << "Key does not exist!" << endl;
            return 0;
        }

        //Delete the key by shifting all elements that are above the key down.
        for (int i = pos; i < cursor->size; i++)
        {
            cursor->key[i] = cursor->key[i + 1];
        }
        cursor->size--;

        if (cursor == root) // if it is root node, then make all pointers NULL
        {
            cout << "Deleted " << x.key_value << " "
                 << "from leaf node successfully\n";
            for (int i = 0; i < MAX + 1; i++)
            {
                cursor->ptr[i] = nullptr;
            }
            if (cursor->size == 0) // if all keys are deleted
            {
                cout << "Tree is deleted!" << endl;
                delete[] cursor->key;
                delete[] cursor->ptr;
                delete cursor;
                cout << "Deleted 1"
                     << "\n";
                root = nullptr;
                return 1;
            }
            return 0;
        }

        // Shift the pointer to the adjacent leaf node.
        cursor->ptr[cursor->size] = cursor->ptr[cursor->size + 1];
        cursor->ptr[cursor->size + 1] = nullptr;
        cout << "Deleted " << x.key_value << " "
             << " from leaf node successfully\n";

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
            if (leftNode->size >= (MAX + 1) / 2 + 1)
            {
                // Create space for transferring key.
                for (int i = cursor->size; i > 0; i--)
                {
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
            if (rightNode->size >= (MAX + 1) / 2 + 1)
            {

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
            for (int i = cursor->size, j = 0; j < rightNode->size; i++, j++)
            {
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
    // Cursor cannot be second-last level since findParent() is not called when child
    // node is leaf during insertion.
    if (cursor->isLeaf || (cursor->ptr[0])->isLeaf)
    {
        return NULL;
    }
    // Iterate through each pointer and check if pointer points to child.
    for (int i = 0; i < cursor->size + 1; i++)
    {
        if (cursor->ptr[i] == child)
        {
            parent = cursor;
            return parent;
        }
        // Else try to find parent from children of cursor.
        else
        {
            parent = findParent(cursor->ptr[i], child);
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
            if (lowerBoundKey >= cursor->key[cursor->size - 1].key_value)
            {
                cursor = cursor->ptr[cursor->size];
            }
            else
            {
                int i = 0;
                while (lowerBoundKey >= cursor->key[i].key_value)
                    i++;
                cursor = cursor->ptr[i];
            }
            level++;
        }
        cout << "Number of index nodes accessed: " << numIndexNodes << endl;
        //Here, we are at the leaf nodes. We then traverse through the nodes from the lowerBoundKey to the upperBoundKey.
        int i = 0;
        while (cursor->key[i].key_value < lowerBoundKey)
            i++;
        float totalRating = 0;
        int totalRecords = 0;
        set<uint> s;
        while (cursor->key[i].key_value <= upperBoundKey)
        {
            for (int j = 0; j < cursor->key[i].add_vect.size(); j++)
            {
                float averageRating = (*(Record *)cursor->key[i].add_vect[j]).averageRating;
                totalRating += averageRating;
                intptr_t dataBlockNo = ((intptr_t)(Record *)cursor->key[i].add_vect[j] - (intptr_t)startAddress) / blockSize;
                s.insert(dataBlockNo);
                totalRecords++;
                // get tconst value
                cout << (*(Record *)cursor->key[i].add_vect[j]).tconst << " | "
                     // get record address
                     << (Record *)cursor->key[i].add_vect[j] << " | "
                     // get block no
                     << dataBlockNo << " | "
                     // get block addr
                     << (Record *)((intptr_t)startAddress + (dataBlockNo * blockSize))
                     << endl;
            }
            if (i == cursor->size - 1 && cursor->ptr[cursor->size] != nullptr && cursor->key[i].key_value <= upperBoundKey)
            {
                cursor = cursor->ptr[cursor->size];
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
            if (key >= cursor->key[cursor->size - 1].key_value)
            {
                cursor = cursor->ptr[cursor->size];
            }
            else
            {
                int i = 0;
                while (key >= cursor->key[i].key_value)
                    i++;
                cursor = cursor->ptr[i];
            }
            level++;
        }
        cout << "Number of index nodes accessed: " << numIndexNodes << endl;
        int i = 0;
        while (cursor->key[i].key_value < key)
            i++;
        float totalRating = 0;
        set<uint> s;
        if (cursor->key[i].key_value == key)
        {
            cout << "Number of records: " << cursor->key[i].add_vect.size() << endl;
            for (int j = 0; j < cursor->key[i].add_vect.size(); j++)
            {
                //cout << (*(Record *) cursor->key[i].add_vect[j]).tconst << "\n";
                totalRating += (*(Record *)cursor->key[i].add_vect[j]).averageRating;
                intptr_t dataBlockNo = ((intptr_t)(Record *)cursor->key[i].add_vect[j] - (intptr_t)startAddress) / blockSize;
                s.insert(dataBlockNo);
                // get tconst value
                cout << (*(Record *)cursor->key[i].add_vect[j]).tconst << " | "
                     // get record address
                     << (Record *)cursor->key[i].add_vect[j] << " | "
                     // get block no
                     << dataBlockNo << " | "
                     // get block addr
                     << (Record *)((intptr_t)startAddress + (dataBlockNo * blockSize))
                     << endl;
            }
        }
        else
        {
            cout << "Does not exist!" << endl;
        }
        cout << "Average Rating: " << totalRating / cursor->key[i].add_vect.size() << endl;
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
    return getHeight(cursor->ptr[0]) + 1;
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
        cout << cursor->key[i].key_value << "|";
    }
    cout << endl;
    cout << "First child node: " << endl;
    cursor = cursor->ptr[0];
    for (int i = 0; i < cursor->size; i++)
    {
        cout << cursor->key[i].key_value << "|";
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
                    q.push(node->ptr[i]);
                }
            }
            if (!node->isLeaf)
            {
                q.push(node->ptr[node->size]);
            }
            q.pop();
        }
    }
    return numNodes;
}

int BPTree::getMax() {
    return MAX;
}

Node *BPTree::getRoot() {
    return root;
}

BPTree::BPTree() : root{NULL} {}
