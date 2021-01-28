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
    free = true;
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
    totalFreeBlocks = NUM_BLOCKS;
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
int StorageSpace::getTotalFreeBlocks() { return totalFreeBlocks; }
bool StorageSpace::isFull() { return full; }
bool StorageSpace::startOfFrag(int index)
{
    if (index < 0 || index >= NUM_BLOCKS) // index out of bounds
        return false;
    return (listOfBlocks[index]->free && (listOfBlocks[index]->next > index));
}

/* @return True if no space can be inserted (internal error). */
bool StorageSpace::insertSpace(spaceList_t spaceList)
{

    // update variables: firstFreeBlock, lastFreeBlock, totalFreeFragments, totalFreeBlocks, full
    return false;
}

/* 
 * Given the number of blocks required by a file, compute where it should be 
 * stored.
 * 
 * @return A list of spaces that can be occupied by the file, starting from the
 * index [start]. */
spaceList_t StorageSpace::findSpace(int blocks, int start)
{
    spaceList_t list;

    while (blocks > 0) // search through the "linkedlist" of free fragments
    {
        int endOfFrag = listOfBlocks[start]->next;
        int currentFragSize = endOfFrag - start;
        if (blocks < currentFragSize) // remaining file fits into this fragment
        {
            fragment_t frag = std::make_pair(start, start + blocks - 1);
            list.push_back(frag);
            blocks = 0;
        }
        else // remaining file is bigger than the fragment
        {
            blocks -= currentFragSize;
            list.push_back(std::make_pair(start, endOfFrag));
            if (listOfBlocks[start]->next != lastFreeBlock)
                start = listOfBlocks[endOfFrag]->next; // go to the next free fragment
            else                                       // reached the end
                start = firstFreeBlock;
        }
    }

    return list;
}

/* @return True if the range (start, end) is not free space */
bool StorageSpace::deleteSpace(spaceList_t spaceList)
{

    // update variables: firstFreeBlock, lastFreeBlock, totalFreeFragments, totalFreeBlocks, full
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
        if (listOfBlocks[i]->next != lastFreeBlock)
            i = listOfBlocks[listOfBlocks[i]->next]->next;
        else
            i = listOfBlocks[i]->next;
        // count++;
    }
}