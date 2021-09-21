using namespace std;

#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <tuple>
#include <typeinfo>
#include <unordered_map>
#include <vector>

#include "b-plus-tree.h"
#include "memory-pool.h"

typedef unsigned int uint;
typedef unsigned char uchar;

int main() {
  cout << "\n---------------- Welcome to the Database Management System "
          "----------------\n"
          "We shall write a short summary of the implementation of this "
          "database here.\n"
       << "\n";

  cout << "Data reading in progress...\n";

  // Reading of TSV file
  ifstream data_file("data/data_short.tsv");

  // Initialise memory pool
  MemoryPool memory_pool{100000000, 100};
  vector<tuple<void *, uint>> data;
  bool start_of_file_flag = true;

  // Data reading process
  if (data_file.is_open()) {

    string line;

    while (getline(data_file, line)) {
      if (start_of_file_flag) {
        start_of_file_flag = false;
        continue;
      }

      Record record;
      string tempLine;

      // copy bytes till we encounter tabspace
      strcpy(record.tconst, line.substr(0, line.find('\t')).c_str());

      stringstream linestream(line);
      getline(linestream, tempLine, '\t');

      linestream >> record.averageRating >> record.numVotes;
      tuple<void *, uint> dataRecord = memory_pool.writeRecord(sizeof(record));
      data.push_back(dataRecord);

      // void * pointer stores the address of the block, but in order to perform
      // pointer arithmetic we have to cast into uint or uchar pointer.

      // memcpy may seem to be potentially dangerous, but it is safe in this
      // usage as it is the only function that allows copying of byte data and
      // it is being performed inside the container.

      void *rcdAdr = (uchar *)get<0>(dataRecord) + get<1>(dataRecord);
      memcpy(rcdAdr, &record, sizeof(record));
    }

    cout << "Reading completed!\n";

    cout << "\n---------------- Database Statistics ----------------\n";
    cout << "1. Size of Memory Pool: " << memory_pool.getPoolSize() << "\n";
    cout << "2. Size of One block: " << memory_pool.getBlockSize() << "B\n";
    cout << "3. Initial Number of Blocks: "
         << memory_pool.getPoolSize() / memory_pool.getBlockSize() << "\n";
    cout << "4. Number of Allocated Blocks: " << memory_pool.getBlocksAssigned()
         << "\n";
    cout << "5. Number of Available Blocks Left: "
         << memory_pool.getBlocksAvailable() << "\n"
         << '\n';

    data_file.close();
  }

  // key:value pair to hold pair of memory addresses.
  unordered_map<void *, void *> memory_block;

  // vector of tuples of record addresses <block address, relative record
  // address>
  vector<tuple<void *, uint>>::iterator records_iterator;

  // Initialise new B+ tree node
  BPTree root_node;
  int search_index;
  int i = 0;

  cout << "Inserting records into B+ tree in progress...\n";

  // Insert records into B+ tree
  for (records_iterator = data.begin(); records_iterator != data.end();
       ++records_iterator) {

    void *blockAddress = (uchar *)get<0>(*records_iterator);
    uint offset = get<1>(*records_iterator);

    if (memory_block.find(blockAddress) == memory_block.end()) {
      void *main_memory_block = operator new(memory_pool.getBlockSize());
      memcpy(main_memory_block, blockAddress, memory_pool.getBlockSize());

      memory_block[blockAddress] = main_memory_block;
    }

    void *recordAddress = (uchar *)memory_block.at(blockAddress) + offset;
    float num = (*(Record *)recordAddress).averageRating;

    keys_struct key;
    key.key_value = num;
    key.add_vect.push_back((uchar *)memory_block.at(blockAddress) + offset);
    root_node.insert(key);

    i++;
  }

  cout << "Insertion into B+ tree completed!\n";

  // Get B+ tree details
  int count = 0;

  cout << "\n";
  cout << "B+ Tree Root: " << root_node.getRoot() << "\n";
  cout << "B+ Tree Height: " << root_node.height(root_node.getRoot()) << "\n";

  cout << "\n---------------- B+ Tree ----------------\n";
  cout << root_node.display(root_node.getRoot(), count, true) << "\n";
  cout << "\n";

  root_node.search(9, true, false);

  keys_struct key;
  key.key_value = 4.1;
  key.add_vect.push_back((uchar *)nullptr);

  // Error from this line onwards
  root_node.remove(key);

  cout << root_node.display(root_node.getRoot(), count, true) << "\n";

  cout << root_node.display(root_node.getRoot(), count, true) << "\n";

  return 0;
}
