/*
 * Yvonne Yang
 * 01/28/2021
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

void StorageSpace::BlockInfo::reset()
{
    free = false;
    prev = NULLI;
    next = NULLI;
}

/* Implementation of StorageSpace */
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
    listOfBlocks[0]->free = true;           // only mark the start and end of the fragment
    listOfBlocks[NUM_BLOCKS - 1]->free = true;
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

/*
 * A safe way to validate the block index input by the user while saving
 * a file.
 * 
 * @return Whether a block at [index] is the start of a free fragment
 */
bool StorageSpace::startOfFrag(int index)
{
    if (index < 0 || index >= NUM_BLOCKS) // index out of bounds
        return false;
    return (listOfBlocks[index]->free && (listOfBlocks[index]->next > index));
}

/* 
 * A more dangerous alternative to the isFragStart(index) input validation,
 * but offers the user more control, as they can choose whichever block in the
 * storage.
 * 
 * @return Whether a block at [index] is free.
 * */
bool StorageSpace::isFree(int index)
{
    if (index < 0 || index >= NUM_BLOCKS) // index out of bounds
        return false;
    if (totalFreeBlocks == NUM_BLOCKS) // all empty
        return true;
    int start = firstFreeBlock;
    while (listOfBlocks[start]->next != lastFreeBlock)
    {
        if (index > start && index < listOfBlocks[start]->next)
        {
            return true;
            ;
        }
        if (start > index)
            break;
    }
    return false;
}

/* 
 * Free up space in the storage when a file is deleted.
 * 
 * @param The memory fragments that store the file.
 * @return True if no space can be inserted (internal error).
 *  */
bool StorageSpace::freeUpSpace(fragList_t fragList)
{
    if (totalFreeBlocks == NUM_BLOCKS) // It's already empty. What?
        return true;

    // handle special case: one huge file that takes up the entire storage
    if (isFull() && fragList.begin()->first == 0 && fragList.begin()->second == NUM_BLOCKS - 1) // releasing all blocks! wheee
    {
        // return to initial state
        listOfBlocks[0]->next = NUM_BLOCKS - 1; // link the first block to the last
        listOfBlocks[NUM_BLOCKS - 1]->prev = 0; // and the last block to the first
        listOfBlocks[0]->free = true;           // only mark the start and end of the fragment
        listOfBlocks[NUM_BLOCKS - 1]->free = true;
        full = false;
        totalFreeFragments = 1;
        firstFreeBlock = 0;
        lastFreeBlock = NUM_BLOCKS - 1;

        return false;
    }

    for (auto fileFrag : fragList) // for each fragment in the list
    {
        int fileFragStart = fileFrag.first;
        int fileFragEnd = fileFrag.second;

        if (fileFragStart < firstFreeBlock) // fileFrag is to the left of all free fragments
        {
            if (!listOfBlocks[fileFragEnd + 1]->free) // surrounded by other files on both sides
            {
                int rightFragStart = firstFreeBlock;
                // connect left end of the right fragment with the right end of this file fragment
                listOfBlocks[rightFragStart]->prev = fileFragEnd;
                listOfBlocks[fileFragEnd]->next = rightFragStart;
                // make this file fragment a free fragment
                listOfBlocks[fileFragStart]->free = true;
                listOfBlocks[fileFragEnd]->free = true;

                firstFreeBlock = fileFragStart; // update
                totalFreeFragments++;
            }
            else // file is adjacent to a free frag on the right
            {
                listOfBlocks[fileFragStart]->free = true;
                listOfBlocks[fileFragStart]->next = listOfBlocks[firstFreeBlock]->next;
                listOfBlocks[listOfBlocks[firstFreeBlock]->next]->prev = fileFragStart;
                listOfBlocks[firstFreeBlock]->reset();
                if (fileFragEnd != fileFragStart) // if file takes up more than one block
                    listOfBlocks[fileFragEnd]->reset();

                firstFreeBlock = fileFragStart;
            }
        }
        else if (fileFragEnd > lastFreeBlock) // fileFrag is to the right of all free fragments
        {

            if (!listOfBlocks[fileFragStart - 1]->free) // surrounded by other files on both sides
            {
                int leftFragEnd = lastFreeBlock;

                // connect right end of the left fragment with the left end of this file fragment
                listOfBlocks[leftFragEnd]->next = fileFragStart;
                listOfBlocks[fileFragStart]->prev = leftFragEnd;
                // make this fragment free
                listOfBlocks[fileFragEnd]->free = true;
                listOfBlocks[fileFragStart]->free = true;

                lastFreeBlock = fileFragEnd; // update
                totalFreeFragments++;
            }
            else // adjacent to a free frag
            {
                listOfBlocks[fileFragEnd]->prev = listOfBlocks[lastFreeBlock]->prev;
                listOfBlocks[listOfBlocks[lastFreeBlock]->prev]->next = fileFragEnd;
                listOfBlocks[lastFreeBlock]->reset();
                if (fileFragEnd != fileFragStart) // if file takes up more than one block
                    listOfBlocks[fileFragStart]->reset();

                lastFreeBlock = fileFragEnd;
            }
        }
        else // file fragment is somewhere in between two free spaces
        {
            if (fileFragStart > 0 && !listOfBlocks[fileFragStart - 1]->free && fileFragEnd < NUM_BLOCKS && !listOfBlocks[fileFragEnd + 1]->free) // surrounded by other files on both sides
            {
                int leftFragEnd = fileFragStart - 1; // find the nearest free fragment on the left
                while (!listOfBlocks[leftFragEnd]->free)
                {
                    leftFragEnd = listOfBlocks[leftFragEnd]->prev - 1;
                }
                int rightFragStart = listOfBlocks[leftFragEnd]->next;

                // insert into linked list
                listOfBlocks[fileFragStart]->free = true;
                listOfBlocks[fileFragEnd]->free = true;
                listOfBlocks[leftFragEnd]->next = fileFragStart;
                listOfBlocks[fileFragStart]->prev = leftFragEnd;
                listOfBlocks[rightFragStart]->prev = fileFragEnd;
                listOfBlocks[fileFragEnd]->next = rightFragStart;

                totalFreeFragments++; //update
            }
            else if (fileFragStart > 0 && !listOfBlocks[fileFragStart - 1]->free && fileFragEnd < NUM_BLOCKS && listOfBlocks[fileFragEnd + 1]->free) // space on the right
            {
                int rightFragStart = fileFragEnd + 1; // find the nearest free fragment
                while (!listOfBlocks[rightFragStart]->free)
                {
                    rightFragStart = listOfBlocks[rightFragStart]->prev - 1;
                }
                int leftFragEnd = listOfBlocks[rightFragStart]->prev;
                // reorganize pointers
                listOfBlocks[fileFragStart]->free = true;
                listOfBlocks[fileFragStart]->prev = leftFragEnd;
                listOfBlocks[leftFragEnd]->next = fileFragStart;
                listOfBlocks[rightFragStart]->reset();
                if (fileFragEnd != fileFragStart) // if file takes up more than one block
                    listOfBlocks[fileFragEnd]->reset();
            }
            else if (fileFragStart > 0 && listOfBlocks[fileFragStart - 1]->free && fileFragEnd < NUM_BLOCKS && !listOfBlocks[fileFragEnd + 1]->free) // space on the left
            {
                int leftFragEnd = fileFragStart - 1; // find the nearest free fragment on the left
                while (!listOfBlocks[leftFragEnd]->free)
                {
                    leftFragEnd = listOfBlocks[leftFragEnd]->prev - 1;
                }
                int rightFragStart = listOfBlocks[leftFragEnd]->next;
                // reorganize pointers
                listOfBlocks[fileFragEnd]->free = true;
                listOfBlocks[fileFragEnd]->next = rightFragStart;
                listOfBlocks[rightFragStart]->prev = fileFragEnd;
                listOfBlocks[leftFragEnd]->reset();
                if (fileFragEnd != fileFragStart) // if file takes up more than one block
                    listOfBlocks[fileFragStart]->reset();
            }
            else // surrounded by spaces
            {
                int leftFragEnd = fileFragStart - 1; // find the nearest free fragment on the left
                while (!listOfBlocks[leftFragEnd]->free)
                {
                    leftFragEnd = listOfBlocks[leftFragEnd]->prev - 1;
                }
                int leftFragStart = listOfBlocks[leftFragEnd]->prev;
                int rightFragStart = listOfBlocks[leftFragEnd]->next;
                int rightFragEnd = listOfBlocks[rightFragStart]->next;

                // connect the left end of the left fragment with the right end of the right fragment to make a big free fragment
                listOfBlocks[leftFragStart]->next = rightFragEnd;
                listOfBlocks[rightFragEnd]->prev = leftFragStart;
                // reset the rest of the blocks
                listOfBlocks[leftFragEnd]->reset();
                listOfBlocks[rightFragStart]->reset();
                listOfBlocks[fileFragEnd]->reset();
                listOfBlocks[fileFragStart]->reset();
            }
        }
        totalFreeBlocks += fileFragEnd - fileFragStart + 1;

        if (DEV) // debugging
            std::cout << "free fragments: " << totalFreeFragments << " free blocks: " << totalFreeBlocks << std::endl;
    }

    if (totalFreeBlocks == 0)
        full = true;

    return false;
}

/* 
 * Given the number of blocks required by a file, compute where it should be 
 * stored.
 * 
 * @return A list of spaces that can be occupied by the file, starting from the
 * index [start]. */
fragList_t StorageSpace::findSpace(int blocks, int start)
{
    fragList_t list;

    while (blocks > 0) // search through the "linkedlist" of free fragments
    {
        int endOfFrag = listOfBlocks[start]->next;
        int currentFragSize = endOfFrag - start + 1;
        if (blocks < currentFragSize) // remaining file fits into this fragment
        {
            fragment_t frag(start, start + blocks - 1);
            list.push_back(frag);
            blocks = 0;
        }
        else // remaining file is bigger than the fragment
        {
            blocks -= currentFragSize;
            list.push_back(std::make_pair(start, endOfFrag));
            if (listOfBlocks[start]->next != lastFreeBlock)
            {
                start = listOfBlocks[endOfFrag]->next; // go to the next free fragment
            }
            else // reached the end, wrap around
            {
                start = firstFreeBlock;
            }
        }
    }

    /*
    if (DEV) // debug
    {
        for (auto it : list)
            std::cout << it.first << ".." << it.second << ", ";
        std::cout << std::endl;
    }
    */
    return list;
}

/* 
 * Removes space from the storage to store a file.
 * 
 * @return True if a fragment in the list is not free space 
 * or if all space is occupied.
 * */
bool StorageSpace::occupySpace(fragList_t fragList)
{
    // all space occupied, huh?
    if (isFull())
    {
        return true;
    }

    // handle file save into an empty storage (where file size is smaller)
    if (totalFreeBlocks == NUM_BLOCKS)
    {
        int fileFragStart = fragList.begin()->first;
        int fileFragEnd = fragList.begin()->second;
        // can only insert at index 0
        listOfBlocks[0]->reset();
        int newFreeFragStart = fileFragEnd - fileFragStart + 1;
        listOfBlocks[newFreeFragStart]->free = true;
        listOfBlocks[newFreeFragStart]->next = NUM_BLOCKS - 1;
        listOfBlocks[NUM_BLOCKS - 1]->prev = newFreeFragStart;
        // connect file fragment
        listOfBlocks[fileFragEnd]->prev = fileFragStart;
        listOfBlocks[fileFragStart]->next = fileFragEnd;

        totalFreeBlocks -= fileFragEnd - fileFragStart + 1;
        firstFreeBlock = newFreeFragStart;
        return false;
    }

    // handle special case: one huge file that takes up the entire remaining storage
    if (totalFreeFragments == 1 && (fragList.begin()->second == lastFreeBlock && fragList.begin()->first == firstFreeBlock)) // one file to rule 'em all
    {
        // first and last block are connected already, just set them to occupied
        listOfBlocks[firstFreeBlock]->free = false;
        listOfBlocks[lastFreeBlock]->free = false;

        // update database info
        full = true;
        totalFreeFragments = 0;
        totalFreeBlocks = 0;
        firstFreeBlock = NULLI;
        lastFreeBlock = NULLI;

        return false;
    }

    for (auto fileFrag : fragList) // for each fragment in the list
    {
        int fileFragStart = fileFrag.first;
        int fileFragEnd = fileFrag.second;
        int freeFragStart = fileFragStart;
        int freeFragEnd = listOfBlocks[freeFragStart]->next;
        /* for use with the isFree() options: search for the surrounding free fragment
        int freeFragStart = NULLI, freeFragEnd = NULLI;
        if (fileFragStart >= firstFreeBlock) // search left
        {
            for (int i = fileFragStart; i >= firstFreeBlock; i--)
            {
                if (listOfBlocks[i]->free)
                {
                    freeFragStart = i;
                    freeFragEnd = listOfBlocks[freeFragStart]->next;
                    break;
                }
            }
        }
        else // search right
        {
            for (int i = fileFragEnd; i <= lastFreeBlock; i++)
            {
                if (listOfBlocks[i]->free)
                {
                    freeFragEnd = i;
                    freeFragStart = listOfBlocks[freeFragEnd]->prev;
                    break;
                }
            }
        }
        */
        if (DEV && freeFragStart == NULLI && freeFragEnd == NULLI) // no fragments??
            std::cout << "Internal error, what's going on??" << std::endl;

        // reorganize pointers
        if (fileFragStart == firstFreeBlock) // surrounding free fragment is the first free space
        {
            if (fileFragEnd == freeFragEnd && fileFragStart == freeFragStart) // file fragment takes up the entire free fragment
            {
                totalFreeFragments--;

                int nextFreeFragStart = listOfBlocks[freeFragEnd]->next;
                listOfBlocks[nextFreeFragStart]->prev = NULLI; // disconnect from the next free frag
                listOfBlocks[fileFragEnd]->next = NULLI;
                firstFreeBlock = nextFreeFragStart;

                listOfBlocks[freeFragStart]->free = false; // mark as occupied
                listOfBlocks[freeFragEnd]->free = false;

                firstFreeBlock = nextFreeFragStart;
            }
            else // file fragment takes up the first section of the free fragment
            {
                // form the new free fragment
                int newFreeFragStart = fileFragEnd + 1;

                listOfBlocks[newFreeFragStart]->free = true;
                listOfBlocks[newFreeFragStart]->next = freeFragEnd;
                listOfBlocks[freeFragEnd]->prev = newFreeFragStart;

                // form the file fragment
                listOfBlocks[fileFragStart]->free = false;
                listOfBlocks[fileFragEnd]->free = false;
                listOfBlocks[fileFragEnd]->prev = fileFragStart;
                listOfBlocks[fileFragStart]->next = fileFragEnd;

                firstFreeBlock = newFreeFragStart; // update
            }
            return false;
        }
        else if (fileFragEnd == lastFreeBlock) // surrounding free fragment is the last free space
        {
            if (fileFragEnd == freeFragEnd && fileFragStart == freeFragStart) // file fragment takes up the entire free fragment
            {
                int prevFreeFragEnd = listOfBlocks[freeFragStart]->prev;
                listOfBlocks[prevFreeFragEnd]->next = NULLI;
                listOfBlocks[fileFragStart]->prev = NULLI;
                lastFreeBlock = prevFreeFragEnd;

                listOfBlocks[freeFragStart]->free = false;
                listOfBlocks[freeFragEnd]->free = false;

                totalFreeFragments--;
            }
            else // file fragment takes up the first section of the free fragment
            {
                // form the new free fragment
                int prevFreeFragEnd = listOfBlocks[fileFragStart]->prev;
                int newFragStart = fileFragEnd + 1;
                listOfBlocks[newFragStart]->free = true;
                listOfBlocks[newFragStart]->next = freeFragEnd;
                listOfBlocks[freeFragEnd]->prev = newFragStart;
                listOfBlocks[prevFreeFragEnd]->next = freeFragStart;
                listOfBlocks[freeFragStart]->prev = prevFreeFragEnd;

                // form the file fragment
                listOfBlocks[fileFragStart]->free = false;
                listOfBlocks[fileFragEnd]->free = false;
                listOfBlocks[fileFragEnd]->prev = fileFragStart;
                listOfBlocks[fileFragStart]->next = fileFragEnd;

                lastFreeBlock = newFragStart; // update
            }
        }
        else // surrounding free fragment is neither first nor last
        {
            // mark file fragment as occupied and connect
            listOfBlocks[fileFragStart]->free = false;
            listOfBlocks[fileFragEnd]->free = false;
            listOfBlocks[fileFragStart]->next = fileFragEnd;
            listOfBlocks[fileFragEnd]->prev = fileFragStart;

            if (fileFragEnd == freeFragEnd && fileFragStart == freeFragStart) // file fragment takes up the entire free fragment
            {
                // connect free fragments
                int leftEnd = listOfBlocks[freeFragStart]->prev;
                int rightStart = listOfBlocks[freeFragEnd]->next;
                listOfBlocks[leftEnd]->next = rightStart;
                listOfBlocks[rightStart]->prev = leftEnd;

                totalFreeFragments--;
            }
            else // file takes up a portion of the free fragment
            {
                int leftEnd = listOfBlocks[freeFragStart]->prev;
                int newFragStart = fileFragEnd + 1;
                // connect new free fragment
                listOfBlocks[newFragStart]->next = freeFragEnd;
                listOfBlocks[freeFragEnd]->prev = newFragStart;
                listOfBlocks[newFragStart]->prev = leftEnd;
                listOfBlocks[leftEnd]->next = newFragStart;
            }
        }

        totalFreeBlocks -= fileFragEnd - fileFragStart + 1;
    }
    /////// debug
    if (DEV)
        std::cout << "free fragments: " << totalFreeFragments << " free blocks: " << totalFreeBlocks << std::endl;
    // update variables: firstFreeBlock, lastFreeBlock, totalFreeFragments, totalFreeBlocks, full
    return false;
}

/* 
 * Print the current free memory blocks in intervals.
 * 
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