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
 * TODO: insertSpace, findSpace, deleteSpace 
 * */
#include <iostream>

#include "StorageSpace.h"

/* Implementation of inner class BlockInfo */
StorageSpace::BlockInfo::BlockInfo() // all blocks are free and point to nothing initially
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

StorageSpace::StorageSpace()
{
    firstFreeBlock = 0;
    lastFreeBlock = NUM_BLOCKS - 1;
    totalFreeFragments = 1;
    full = false;

    listOfBlocks = new BlockInfo *[NUM_BLOCKS];

    // initialize all blocks
    for (int i = 0; i < NUM_BLOCKS; i++)
    {
        listOfBlocks[i] = new BlockInfo(); // all blocks are free and point to nothing initially
    }

    listOfBlocks[0]->next = NUM_BLOCKS - 1; // link the first block to the last
    listOfBlocks[NUM_BLOCKS - 1]->prev = 0; // and the last block to the first
}

StorageSpace::~StorageSpace()
{
    for (int i = 0; i < NUM_BLOCKS; i++)
    {
        delete listOfBlocks[i];
    }
    delete[] listOfBlocks;
}

/* Accessors */
int StorageSpace::getTotalFragments() { return totalFreeFragments; }
bool StorageSpace::isFull() { return full; }
bool StorageSpace::startOfFrag(int index) { return (listOfBlocks[index]->free && (listOfBlocks[index]->next > index)); }

/* @return True if no space can be inserted (internal error). */
bool StorageSpace::insertSpace(spaceList_t spaceList)
{

    return false;
}

/* @return A list of spaces that can be occupied by a file of size bytes
 * (in bytes, obviously), starting from the index [start]. */
spaceList_t StorageSpace::findSpace(int bytes, int start)
{
    int blocks = (double)bytes / 1024.0 / BLOCK_SIZE;
    spaceList_t list;

    return list;
}

/* @return True if the range (start, end) is not free space */
bool StorageSpace::deleteSpace(spaceList_t spaceList)
{

    // update variables: firstFreeBlock, lastFreeBlock, totalFree, full
    return false;
}

/* 
 * extra feature: number fragments for easier user input
 * */
void StorageSpace::printFreeSpace()
{
    // int count = 1; // give each fragment an index so that it's easier for the user
    auto i = firstFreeBlock;
    while (i != lastFreeBlock)
    {
        // std::cout << "Fragment " << count << ": ";
        std::cout << i << ".." << listOfBlocks[i]->next << std::endl;
        i = listOfBlocks[listOfBlocks[i]->next]->next;
        // count++;
    }
}