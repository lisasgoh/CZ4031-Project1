using namespace std;

#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <tuple>
#include <typeinfo>
#include <unordered_map>
#include <vector>

// Import other header files from directory
#include "b-plus-tree.h"
#include "memory-pool.h"

typedef unsigned int uint;
typedef unsigned char uchar;
void *startAddress = NULL;

void experiment_2(BPTree root, int count);
void experiment_3(BPTree root, int numVotes);
void experiment_4(BPTree root, int numVotes_1, int numVotes_2);
void experiment_5(BPTree root, int numVotes, int count);

// Main program
int main() {
  
     cout << "\n------------ Welcome to the Database Management System "
               "------------\n"
               "This is a simulation of the database management system, to\n"
               "demonstrate storage and indexing of a database, designed by "
               "Group "
               "8.\n"
          << "\n";

     // Storage Implementation
     cout << "Data reading in progress...\n";

     ifstream data_file("data/data_short.tsv");

     // Initialise memory pool
     // MemoryPool::MemoryPool(uint poolSize, uint blockSize)
     MemoryPool memory_pool{100000000, 100};

     // Initialise variables for data storage operations
     vector<tuple<void *, uint>> data;
     bool start_of_file_flag = true;
     string copied;
     Record record;
     string tempLine;

     // Data reading process
     if (data_file.is_open()) {
          // Serialisation of the TSV file into bytes
          // Iterate 'data_file' and store individual bytes of data into
          // 'copied'
          while (getline(data_file, copied)) {
          if (start_of_file_flag) {
               start_of_file_flag = false;
               continue;
          }

          // Copy the data into the block of records - Copy bytes from
          // 'copied' to 'record', untill we encounter tabspace
          strcpy(record.tconst, copied.substr(0, copied.find('\t')).c_str());

          stringstream line_stream(copied);
          getline(line_stream, tempLine, '\t');

          // Write the blocks of records into disk, with the attributes in
          // the dataset
          line_stream >> record.averageRating >> record.numVotes;
          tuple<void *, uint> data_record =
               memory_pool.writeRecord(sizeof(record));
          data.push_back(data_record);

          // void * pointer to store the address of the blocks in the
          // memory Cast pointer into uint or uchar pointer to perform
          // pointer arithmetic
          void *rcdAdr = (uchar *)get<0>(data_record) + get<1>(data_record);
          memcpy(rcdAdr, &record, sizeof(record));
          }

          cout << "Reading completed!\n"; // Storage implementation completed

          // Print database staistics (Experiment 1)
          cout << "\n---------------- Experiment 1: Database Statistics ----------------\n";
          cout << "1. Size of Database: " << memory_pool.getPoolSize() << "\n";
          cout << "2. Size of One block: " << memory_pool.getBlockSize() << "B\n";
          cout << "3. Initial Number of Blocks: "
               << memory_pool.getPoolSize() / memory_pool.getBlockSize() << "\n";
          cout << "4. Number of Allocated Blocks: "
               << memory_pool.getBlocksAssigned() << "\n";
          cout << "5. Number of Available Blocks Left: "
               << memory_pool.getBlocksAvailable() << "\n"
               << '\n';

          data_file.close();
     }

     // Indexing Implementation

     // key:value pair to hold pairs of memory addresses.
     unordered_map<void *, void *> memory_block;

     // vector of tuples of record addresses <block address, relative record
     // address>
     vector<tuple<void *, uint>>::iterator records_iterator;

     // Initialise new B+ tree node and other variables for indexing
     BPTree root_node;
     int search_index;
     int iterating_index = 0;

     cout << "Inserting records into B+ tree in progress...\n";

     // Insert records into B+ tree
     for (records_iterator = data.begin(); records_iterator != data.end();
          ++records_iterator) {
          void *blockAddress = (uchar *)get<0>(*records_iterator);
          uint offset = get<1>(*records_iterator);

          // Copy bytes from memory address to database
          if (memory_block.find(blockAddress) == memory_block.end()) {
          void *main_memory_block = operator new(memory_pool.getBlockSize());
          memcpy(main_memory_block, blockAddress, memory_pool.getBlockSize());
          memory_block[blockAddress] = main_memory_block;
          }

          void *recordAddress = (uchar *)memory_block.at(blockAddress) + offset;

          // Insert into B+ tree based on the numVotes attribute
          float num = (*(Record *)recordAddress).numVotes;

          keys_struct key;
          key.key_value = num;
          key.add_vect.push_back((uchar *)memory_block.at(blockAddress) + offset);
          root_node.insert(key);

          iterating_index++;
     }

     cout << "Insertion into B+ tree completed!\n";

     int count = 3;

     // Experiment 2
     experiment_2(root_node, count);

     // Experiment 3
     experiment_3(root_node, 500);

     // Experiment 4
     experiment_4(root_node, 30000, 40000);

     // Experiment 5
     experiment_5(root_node, 1000, count);

     // // Error from this line onwards
     // root_node.remove(key);

     // cout << root_node.display(root_node.getRoot(), count, true) << "\n";

     return 0;
}

void experiment_2(BPTree root, int count){
     // Get B+ tree details
     cout << "\n-------------- Experiment 2: Information on B+ Tree --------------\n";

     cout << "Parameter n of B+ Tree: 5"
          << "\n";
     cout << "Number of Nodes in B+ Tree: " << root.getNumNode() << "\n";
     cout << "B+ Tree Height: " << root.height(root.getRoot()) << "\n";

     cout << "\nB+ Tree:\n";
     cout << root.display(root.getRoot(), count, true) << "\n";
     cout << "\n";
}

void experiment_3(BPTree root, int numVotes){
     cout << "------------- Experiment 3: Search where numVotes = " << numVotes << " -------------\n";

     cout << "These are the index nodes with records which satisfy the condition"
          << "\n";
     root.search(numVotes, true, true);

     cout << "\nData Blocks: "
          << "\n";
     cout << "\nAverage Value for averageRatings: "
          << "\n";
}

void experiment_4(BPTree root, int numVotes_1, int numVotes_2){
     cout << "\n-------- Experiment 4: Search where " << numVotes_1 << " <= numVotes <= " << numVotes_2 << " --------\n";

     cout << "These are the index nodes with records which satisfy the condition"
          << "\n";
     // root_node.search(500, true, true);

     cout << "\nData Blocks: "
          << "\n";
     cout << "\nAverage Value for averageRatings: "
          << "\n";
}

void experiment_5(BPTree root, int numVotes, int count){
     cout << "\n-------- Experiment 5: Delete Movies where numVotes = " << numVotes << " --------\n\n";

     // Search records based on condition
     cout << "These are the index nodes with records which satisfy the condition\n";
     root.search(numVotes, true, true);
     cout << '\n';

     cout << "Deleting records where numVotes = " << numVotes << " in progress...\n\n";

     keys_struct key;
     key.key_value = 115;
     key.add_vect.push_back((uchar *)nullptr);

     int numNodeBeforeRemoval = root.getNumNode();
     int numNodeMerged = root.remove(key);
     int numNodeAfterRemoval = root.getNumNode();
     int numNodeRemoved = numNodeBeforeRemoval - numNodeAfterRemoval;

     cout << "\nNumber of Nodes Deleted: " << numNodeRemoved << "\n";
     cout << "Number of Nodes Merged: " << numNodeMerged << "\n";

     cout << "\nNumber of Nodes in Updated B+ Tree: " << root.getNumNode()
          << "\n";
     cout << "Height of Updated B+ Tree: " << root.height(root.getRoot()) 
          << "\n";

     
     cout << "\nB+ Tree:\n";
     cout << root.display(root.getRoot(), count, true) << "\n";
     cout << "\n";
}
