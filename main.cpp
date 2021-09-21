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

// Main program
int main() {
     cout << "\n---------------- Welcome to the Database Managmenet System ----------------\n"
          "We shall write a short summary of the implementation of this database here.\n"
          << "\n";

     cout << "Data reading in progress...\n";

     // Data reading process

     // Reading of TSV file
     ifstream dataFile("data/data_short.tsv");

     // Initialise memory pool
     MemoryPool memPool{100000000, 100};
     vector < tuple<void *, uint> > dataset;
     bool flag = true;

     if (dataFile.is_open()) {

          string line;

          while (getline(dataFile, line)) {
               if (flag) {
                    flag = false;
                    continue;
               }

               Record record;
               string tempLine;

               // copy bytes till we encounter tabspace
               strcpy(record.tconst, line.substr(0, line.find('\t')).c_str());

               stringstream linestream(line);
               getline(linestream, tempLine, '\t');

               linestream >> record.averageRating >> record.numVotes;
               tuple<void *, uint> dataRecord = memPool.writeRecord(sizeof(record));
               dataset.push_back(dataRecord);

               // void * pointer stores the address of the block, but in order to
               // perform pointer arithmetic we have to cast into uint or uchar
               // pointer.

               // memcpy may seem to be potentially dangerous, but it is safe in
               // this usage as it is the only function that allows copying of byte
               // data and it is being performed inside the container.

               void *rcdAdr = (uchar *)get<0>(dataRecord) + get<1>(dataRecord);
               memcpy(rcdAdr, &record, sizeof(record));
          }

          cout << "Reading completed!\n";

          cout << "\n---------------- Database Statistics ----------------\n";
          cout << "1. Size of Memory Pool: " << memPool.getPoolSize() << "\n";
          cout << "2. Size of One block: " << memPool.getBlockSize() << "B\n";
          cout << "3. Initial Number of Blocks: "
               << memPool.getPoolSize() / memPool.getBlockSize() << "\n";
          cout << "4. Number of Allocated Blocks: " << memPool.getBlocksAssigned() << "\n";
          cout << "5. Number of Available Blocks Left: " << memPool.getBlocksAvailable() << "\n"
               << '\n';

          dataFile.close();
     }

     // key:value pair to hold pair of memory addresses.
     unordered_map <void *, void *> blocksInMemory;

     // vector of tuples of record addresses <block address, relative record address>
     vector <tuple <void *, uint>> :: iterator recordsIterator;

     // Initialise new B+ tree node
     BPTree node;
     int search_index;
     int i = 0;

     // Insert records into B+ tree
     for (recordsIterator = dataset.begin(); recordsIterator != dataset.end(); ++recordsIterator) {

          void *blockAddress = (uchar *) get <0> (*recordsIterator);
          uint offset = get<1>(*recordsIterator);

          if (blocksInMemory.find(blockAddress) == blocksInMemory.end()) {
               void *mainMemoryBlock = operator new(memPool.getBlockSize());
               memcpy(mainMemoryBlock, blockAddress, memPool.getBlockSize());

               blocksInMemory[blockAddress] = mainMemoryBlock;
          }

          void *recordAddress = (uchar *) blocksInMemory.at(blockAddress) + offset;
          float num = (*(Record *)recordAddress).averageRating;

          keys_struct key;
          key.key_value = num;
          key.add_vect.push_back((uchar *) blocksInMemory.at(blockAddress) + offset);
          node.insert(key);

          i++;
     }

     // Get B+ tree details
     int count = 0;

     cout << "\n";
     cout << "B+ Tree Root: " << node.getRoot() << "\n";
     cout << "B+ Tree Height: " << node.height(node.getRoot()) << "\n";

     cout << "\n---------------- B+ Tree ----------------\n";
     cout << node.display(node.getRoot(), count, true) << "\n";
     cout << "\n";

     node.search(9, true, false);

     keys_struct key;
     key.key_value = 4.1;
     key.add_vect.push_back((uchar *) nullptr);
     
     // Error from this line onwards
     node.remove(key);

     cout << node.display(node.getRoot(), count, true) << "\n";

     return 0;
}
