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
uint blockSize = 100;

void experiment_2(BPTree root);
void experiment_3(BPTree root, int numVotes);
void experiment_4(BPTree root, int numVotes_1, int numVotes_2);
void experiment_5(BPTree root, int numVotes);

// Main program
int main() {

    cout << "\n------------ Welcome to the Database Management System "
            "------------\n"
            "This is a simulation of the database management system, to "
            "demonstrate storage and indexing of a database, designed by Group "
            "8.\n"
         << "\n";


    // Storage Implementation
    cout << "Data reading in progress...\n";

    ifstream data_file("data/data.tsv");

    // Initialise memory pool
    // MemoryPool::MemoryPool(uint poolSize, uint blockSize)
    MemoryPool memory_pool{100000000, blockSize};

    // Initialise variables for data storage operations
    vector<tuple<void *, uint>> data;
    bool start_of_file_flag = true;
    string copied;
    Record record;
    string tempLine;

    // Data reading process
    if (data_file.is_open()) {

        // Serialisation of the TSV file into bytes
        // Iterate 'data_file' and store individual bytes of data into 'copied'
        while (getline(data_file, copied)) {
            if (start_of_file_flag) {
                start_of_file_flag = false;
                continue;
            }

            // Copy the data into the block of records - Copy bytes from 'copied' to
            // 'record', untill we encounter tabspace
            strcpy(record.tconst, copied.substr(0, copied.find('\t')).c_str());

            stringstream line_stream(copied);
            getline(line_stream, tempLine, '\t');

            // Write the blocks of records into disk, with the attributes in the
            // dataset
            line_stream >> record.averageRating >> record.numVotes;
            tuple<void *, uint> data_record =
                    memory_pool.writeRecord(sizeof(record));
            data.push_back(data_record);

            // void * pointer to store the address of the blocks in the memory
            // Cast pointer into uint or uchar pointer to perform pointer arithmetic
            // <0> is block pointer
            // <1> is block offset
            void *rcdAdr = (uchar *) get<0>(data_record) + get<1>(data_record);
            memcpy(rcdAdr, &record, sizeof(record));
        }

        cout << "Reading completed!\n"; // Storage implementation completed
        // Print database staistics (Experiment 1)
        cout << "\n---------------- Database Statistics ----------------\n";
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

    // vector of tuples of record addresses <block address, relative record
    // address>
    vector<tuple<void *, uint>>::iterator records_iterator;

    // Initialise new B+ tree node and other variables for indexing
    BPTree root_node;
    int search_index;
    int iterating_index = 0;

    cout << "Inserting records into B+ tree in progress...\n";

    int i =1;
    
     float totalRating = 0;
     int countRating = 0;
    // Insert records into B+ tree
    // loop from start of data till end
    for (records_iterator = data.begin(); records_iterator != data.end();
         ++records_iterator) {

        void *blockAddress = (uchar *) get<0>(*records_iterator);
        uint offset = get<1>(*records_iterator);

        // get starting block address
        if (startAddress == NULL){
            startAddress = blockAddress;
        }

        void *recordAddress = (uchar *) blockAddress + offset;
        i++;

        // Insert into B+ tree based on the numVotes attribute
        float num = (*(Record *) recordAddress).numVotes;

        keys_struct key;
        key.key_value = num;
        key.add_vect.push_back((uchar *) blockAddress + offset);
        root_node.insert(key);
       iterating_index++;
    }

  //  root_node.printTree(root_node.getRoot());

  cout << "Insertion into B+ tree completed!\n";

  // Experiment 2
  experiment_2(root_node);

  // Experiment 3
  experiment_3(root_node, 500);

  // Experiment 4
  experiment_4(root_node, 30000, 40000);

  // Experiment 5
  experiment_5(root_node, 1000);

  return 0;
}


void experiment_2(BPTree root) {
  // Get B+ tree details
  cout << "\n-------------- Experiment 2: Information on B+ Tree "
          "--------------\n";

  cout << "Parameter n of B+ Tree: " << root.getMax() << "\n";
  cout << "Number of Nodes in B+ Tree: " << root.calculateNumNodes(root.getRoot()) << "\n";
  cout << "B+ Tree Height: " << root.getHeight(root.getRoot()) << "\n";

  cout << "\nB+ Tree:\n";

  root.display(root.getRoot());
}

void experiment_3(BPTree root, int numVotes) {
  cout << "\n------------- Experiment 3: Search where numVotes = " << numVotes
       << " -------------\n";

  cout << "These are the index nodes with records which satisfy the condition:"
       << "\n";
  root.searchSingle(numVotes);
}

void experiment_4(BPTree root, int numVotes_1, int numVotes_2) {
  cout << "\n-------- Experiment 4: Search where " << numVotes_1
       << " <= numVotes <= " << numVotes_2 << " --------\n";

  cout << "These are the index nodes with records which satisfy the condition:"
       << "\n";
  root.searchRange(numVotes_1, numVotes_2);
}

void experiment_5(BPTree root, int numVotes) {
  cout << "\n-------- Experiment 5: Delete Movies where numVotes = " << numVotes
       << " --------\n\n";

  cout << "Deleting records where numVotes = " << numVotes
       << " in progress...\n\n";

  keys_struct key;
  key.key_value = numVotes;
  key.add_vect.push_back((uchar *)nullptr);

//   root.printTree(root.getRoot());
  int numNodeBeforeRemoval = root.calculateNumNodes(root.getRoot());
  int numNodeMerged = root.remove(key);
  int numNodeAfterRemoval = root.calculateNumNodes(root.getRoot());
  int numNodeRemoved = numNodeBeforeRemoval - numNodeAfterRemoval;

  cout << "\nNumber of Nodes Deleted: " << numNodeRemoved << "\n";

  cout << "\nNumber of Nodes in Updated B+ Tree: " << numNodeAfterRemoval << "\n";
  cout << "Height of Updated B+ Tree: " << root.getHeight(root.getRoot()) << "\n";

  cout << "\nB+ Tree:\n";

  root.display(root.getRoot());
}