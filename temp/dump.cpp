  // Indexing Implementation

  // key:value pair to hold pairs of memory addresses.
//   unordered_map<void *, void *> memory_block;

//   // vector of tuples of record addresses <block address, relative record
//   // address>
//   vector<tuple<void *, uint>>::iterator records_iterator;

//   // Initialise new B+ tree node and other variables for indexing
//   BPTree root_node;
//   int search_index;
//   int iterating_index = 0;

//   cout << "Inserting records into B+ tree in progress...\n";

//   float totalRating = 0;
//   int countRating = 0;

//   float totalRating500 = 0;
//   int count500 = 0;
//   unordered_map<int, int> hmap;
//   // Insert records into B+ tree
//   for (records_iterator = data.begin(); records_iterator != data.end();
//        ++records_iterator) {
//     void *blockAddress = (uchar *)get<0>(*records_iterator);
//     uint offset = get<1>(*records_iterator);

//     // Copy bytes from memory address to database
//     if (memory_block.find(blockAddress) == memory_block.end()) {
//       void *main_memory_block = operator new(memory_pool.getBlockSize());
//       memcpy(main_memory_block, blockAddress, memory_pool.getBlockSize());
//       memory_block[blockAddress] = main_memory_block;
//     }

//     void *recordAddress = (uchar *)memory_block.at(blockAddress) + offset;

//     // Insert into B+ tree based on the numVotes attribute
//    float num = (*(Record *)recordAddress).numVotes;

//    //cout << num << endl;
//     keys_struct key;
//     key.key_value = num;
//     key.add_vect.push_back((uchar *)memory_block.at(blockAddress) + offset);
//     if (num >= 50000 && num <= 100000) {
//           if (hmap.find(num) != hmap.end()) {
//                hmap[num]++;
//           } else {
//                hmap[num] = 1;
//           }
//          totalRating += (*(Record *)recordAddress).averageRating;
//          countRating++;
//     }
//     if (num == 500)
//     {
//          totalRating500 += (*(Record *)recordAddress).averageRating;
//          count500++;
//     }
//     root_node.insert(key);
//     //cout << "Number of nodes: " << root_node.getNumNode() << endl;
//     //root_node.printTree(root_node.getRoot());
//     iterating_index++;
//   }

    // Indexing Implementation

    // key:value pair to hold pairs of memory addresses.
//    unordered_map<void *, void *> memory_block;


// int BPTree::searchSingleCount(float key, unordered_map<int, int> hmap) 
// {
//     int numIndexNodes = 0;
//     if (root == nullptr) 
//     {
//         throw std:: logic_error("Tree is empty!");
//     } 
//     else {
//         Node *cursor = root;
//         //While leaf node is not reached yet.
//         int level = 0;
//         while (!cursor->isLeaf) 
//         {
//             // cout << "Index node at level: " << level << endl;
//             // for (int k = 0; k < cursor->size; k++)
//             // {
//             //     cout << cursor->key[k].key_value << "|";
//             // }
//             // cout << endl;
//             numIndexNodes++; 
//             // Iterate through 
//             if (key >= cursor->key[cursor->size-1].key_value) 
//             {
//                 cursor = cursor->ptr[cursor->size];
//             } 
//             else 
//             {
//                 int i = 0;
//                 while (key >= cursor->key[i].key_value) i++;
//                 cursor = cursor->ptr[i];
//            }
//            level++;
//         }
//         //cout << "Number of index nodes: " << numIndexNodes << endl;
//         int i = 0;
//         // cout << "Key index: " << cursor->key[i].key_value << endl;
//         while (cursor->key[i].key_value < key) i++;
//         // cout << "Key index: " << cursor->key[i].key_value << endl;
//         float totalRating = 0;
//         if (cursor->key[i].key_value == key) 
//         {
//             if (hmap[key] != cursor->key[i].add_vect.size())
//             {
//                  cout << "Expected records for " << key << ": " << hmap[key] << ", actual: " << cursor->key[i].add_vect.size() << endl;
//             }
//             for (int j = 0; j < cursor->key[i].add_vect.size(); j++) {
//                 totalRating +=(*(Record *)cursor->key[i].add_vect[j]).averageRating;
//             }
//             return cursor->key[i].add_vect.size();
//         }
//         else
//         {
//             if (hmap.find(key) != hmap.end())
//             {
//                 cout << key << " Does not exist but should exist" << endl;
//             }
//         }
//         return 0;
//         cout << "Average Rating: " << totalRating / cursor->key[i].add_vect.size() << endl;
//     }
// }
