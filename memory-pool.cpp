#include "memory-pool.h"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

typedef unsigned int uint;
typedef unsigned char uchar;

using namespace std;

/**
 * Constructor for MemoryPool
 */
MemoryPool::MemoryPool(uint poolSize, uint blockSize)
{
    this->poolSize = poolSize;
    this->blockSize = blockSize;
    this->poolPtr = new uchar[poolSize];
    this->blockPtr = nullptr;
    this->blockOffset = 0;
    this->sizeOfAssignedBlocks = 0;
    this->sizeOfAssignedRecords = 0;
    this->numBlocksAssigned = 0;
    this->numBlocksAvailable = poolSize / blockSize;
    this->numRecordsAssigned = 0;
}

/**
 * Destructor for MemoryPool
 */
MemoryPool::~MemoryPool()
{
    delete poolPtr;
    poolPtr = nullptr;
}

/**
 * Assigns a new block in the memory pool
 * 1. If there are no available blocks in the pool, no assignment can be done
 * 2. Advance blockPtr to the starting address of the current block
 * 3. Update the total size of assigned blocks
 * 4. Update the number of available and assigned blocks
 */
bool MemoryPool::assignBlock()
{
    if (numBlocksAvailable == 0)
    {
        cout << "Failed to assign block. Memory is full" << endl;
        return false;
    }
    blockPtr = poolPtr + (numBlocksAssigned * blockSize);
    sizeOfAssignedBlocks += blockSize;
    numBlocksAvailable -= 1;
    numBlocksAssigned += 1;
    blockOffset = 0;
    return true;
}

/**
 * Writes a record to the current block, pointed to by `blockPtr`, in the memory
 * pool
 * 1. If there're no assigned blocks, or the record cannot fit in the current
 * block, assign a new block
 * 2. If the block assignment fails, throw an error
 * 3. If the record size > block size, throw an error, as we're using unspanned
 * records here
 * 4. Create the record to be written in the form with the block it sits in, and
 * its offset
 * 5. Update the total size of assigned records, total number of assigned
 * records, and the current block's offset
 */
tuple<void *, uint> MemoryPool::writeRecord(uint recordSize)
{
    if (numBlocksAssigned == 0 || blockSize < (blockOffset + recordSize))
    {
        if (!assignBlock())
            throw "Failed to write record. No free space in blocks, or no blocks can "
            "be allocated";
    }

    if (recordSize > blockSize)
    {
        throw "Failed to write record. Record size > block size";
    }

    tuple<void *, uint> recordWritten(blockPtr, blockOffset);
    sizeOfAssignedRecords += recordSize;
    blockOffset += recordSize;
    numRecordsAssigned += 1;
    return recordWritten;
}

/**
 * Deletes an existing record from the memory pool
 * 1. Write null values to the range [blockAddress + offset, blockAddress +
 * offset + recordSize]
 * 2. Update the total size of assigned records
 * 3. If the record to be deleted is the first (and only) record in its block,
 * we can free up the block
 */
bool MemoryPool::deleteRecord(uchar *blockAddress, uint offset,
                              const uint recordSize)
{
    try
    {
        fill(blockAddress + offset, blockAddress + offset + recordSize, '\0');
        sizeOfAssignedRecords -= recordSize;

        if (offset == 0)
        {
            sizeOfAssignedBlocks -= blockSize;
            numBlocksAssigned -= 1;
            numBlocksAvailable += 1;
        }

        numRecordsAssigned -= 1;
        return true;
    }

    catch (exception &e)
    {
        cout << e.what() << "\n";
        cout << "Record or block deletion failed"
             << "\n";
        return false;
    }
}
