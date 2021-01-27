/*
 * Yvonne Yang
 * 01/26/2021
 * 
 * Implementation file.
 * This custom-built data structure is a hybrid between an array and a linked 
 * list, and specializes in keeping track of the free memory in the storage.
 *  
 * Each element in the array is a triple--bool, int, int. The first boolean 
 * marks if a block is free (true) or occupied (false). The two ints act as
 * pointers to the index of other blocks. This helps divide the memory into
 * free/occupied sections and chains similar sections for easier access.
 * 
 * TODO: insertSpace, deleteSpace, printFreeSpace
 * */
#include <tuple>

#include "StorageSpace.h"

/* Implementation of inner class BlockInfo */
StorageSpace::BlockInfo::BlockInfo()
{
    free = false;
    prev = NULLI;
    next = NULLI;
}

StorageSpace::BlockInfo::BlockInfo(bool f, int p, int n)
{
    free = f;
    prev = p;
    next = n;
}

bool StorageSpace::BlockInfo::free() { return free; }
int StorageSpace::BlockInfo::prev() { return prev; }
int StorageSpace::BlockInfo::next() { return next; }

void StorageSpace::BlockInfo::setAsFree(bool f)
{
    free = f;
}
void StorageSpace::BlockInfo::setPrev(int p)
{
    prev = p;
}
void StorageSpace::BlockInfo::setNext(int n)
{
    next = n;
}

StorageSpace::StorageSpace()
{
    firstFreeBlock = 0;

    listOfBlocks.fill(std::make_tuple(true, NULLI, NULLI));         // initialize all blocks
    listOfBlocks[0] = std::make_tuple(true, NULLI, NUM_BLOCKS - 1); // link the first block to the last
    listOfBlocks[NUM_BLOCKS] = std::make_tuple(true, 0, NULLI);     // and the last block to the first
}

bool StorageSpace::insertSpace(int start, int end) {}

bool StorageSpace::deleteSpace(int start, int end) {}

void StorageSpace::printFreeSpace()
{
    auto iter = listOfBlocks[firstFreeBlock];
}