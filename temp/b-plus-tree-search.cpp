#include "b_plus_tree.h"

void BPTree::search(float lowerBoundKey, float upperBoundKey) 
{
    if (root == nullptr) 
    {
        throw std:: logic_error("Tree is empty!");
    } 
    else {
        Node *cursor = root;
        //While leaf node is not reached yet.
        while (!cursor->isLeaf) {
            // Iterate through 
            if (lowerBoundKey >= cursor->key[cursor->size-1].key_value) 
            {
                cursor = cursor->ptr[cursor->size];
            } 
            else 
            {
                int i = 0;
                while (lowerBoundKey >= cursor->key[i++]);
                cursor = cursor->ptr[i];
           }
        }
        //Here, we are at the leaf nodes. We then traverse through the nodes from the lowerBoundKey to the upperBoundKey.
        int i = 0;
        while (cursor->key[i++] < lowerBoundKey);
        while (cursor->key[i] <= upperBoundKey) 
        {
            std::cout << "Number of Data Blocks: %d", cursor->key[i].add_vect.size()) << endl;
            for (int j = 0; j < cursor->key[i].add_vect.size(); ++j) {
                //printf("Data Block: ");
                //printf("%p", (uchar *) cursor->key[i].add_vect[j]);
                //printf("\n");
                //printf("tconst: ");
                //cout << (*(Record *) cursor->key[i].add_vect[j]).tconst << "\n";
            }
            if (i == cursor->size && cursor->ptr[cursor->size] != nullptr && cursor->key[i] <= upperBoundKey) {
                cursor = cursor->ptr[cursor->size];
                i = 0;
            } 
            i++;
        }
    }
}