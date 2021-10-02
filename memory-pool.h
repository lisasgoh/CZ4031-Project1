#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H
#include <math.h>

#include <algorithm>
#include <cstring>
#include <iostream>
#include <tuple>
#include <vector>
using namespace std;

typedef unsigned int uint;
typedef unsigned char uchar;

const int MAX = 5;

// Represents a datafile record
struct Record {
  char tconst[10];
  float averageRating;
  uint numVotes;
};

class MemoryPool {
private:
  uchar *poolPtr;   // start address of memory pool
  uchar *blockPtr;  // start address of current block
  uint blockOffset; // offset of current block

  uint blockSize;             // size (in bytes) of 1 block
  uint poolSize;              // size (in bytes) of the entire memory pool
  uint sizeOfAssignedBlocks;  // size (in bytes) of all assigned blocks
  uint sizeOfAssignedRecords; // size (in bytes) of all assigned records

  int numBlocksAssigned;
  int numBlocksAvailable;
  int numRecordsAssigned;

public:
  /**
   * Constructor
   * @param poolSize Buffer pool size
   * @param blockSize Block size
   */
  MemoryPool(uint poolSize, uint blockSize);

  /**
   * Destructor
   */
  ~MemoryPool();

  /**
   * Assign 1 block in the memory pool
   * @return Whether a new block was successfully assigned
   */
  bool assignBlock();

  /**
   * Write the record to the memory pool
   * @param recordSize size (in bytes) of the record to be written
   * @return (starting address of block, offset from starting address)
   */
  tuple<void *, uint> writeRecord(uint recordSize);

  /**
   * Delete the record at `blockAddress` + `offset` in the memory pool
   * @param blockAddress start address of the block containing the record to
   * be deleted
   * @param offset offset relative to the blockAddress
   * @param recordSize size (in bytes) of the record to be deleted
   * @return Whether the record was successfully deleted
   */
  bool deleteRecord(uchar *blockAddress, uint offset, uint recordSize);

  /**
   * Returns the block size
   * @return block size
   */
  uint getBlockSize() { return blockSize; }

  /**
   * Returns the memory pool's size
   * @return memory pool's size
   */
  uint getPoolSize() { return poolSize; }

  /**
   * Returns the number of blocks assigned in memory pool
   * @return number of blocks assigned in memory pool
   */
  int getBlocksAssigned() { return numBlocksAssigned; }

  /**
   * Returns the number of blocks available in memory pool
   * @return number of blocks available in memory pool
   */
  int getBlocksAvailable() { return numBlocksAvailable; }

  /**
   * Returns the number of records assigned in memory pool
   * @return number of records assigned in memory pool
   */
  int getNumberOfRecords() { return numRecordsAssigned; }

  /**
   * Returns the database size in MB
   * @return database size in MB
   */
  int getDatabaseSizeInMB() { return sizeOfAssignedRecords / pow(2, 20); }

  int getMax() { return MAX; }

  uchar *getPoolPtr() { return poolPtr; }
};

#endif // MEMORY_POOL_H
