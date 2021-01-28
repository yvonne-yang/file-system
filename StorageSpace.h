/*
 * Yvonne Yang
 * 01/26/2021
 * 
 * Declaration file.
 * This custom-built data structure is a hybrid between an array and a linked 
 * list, and specializes in keeping track of the free memory in the storage.
 * 
 * Each element in the array is a triple--bool, int, int. The first boolean 
 * marks if a block is free (true) or occupied (false). The two ints act as
 * pointers to the index of other blocks. This helps divide the memory into
 * free/occupied sections and chains similar sections for easier access.
 * 
 * */

#pragma once

#include <vector>
#include <utility>

#include "StorageFragment.h"
#include "globals.h"

typedef std::pair<int, int> fragment_t;      // a start and an end index define\
                                         // a fragment
typedef std::vector<fragment_t> spaceList_t; // a list of spaces

class StorageSpace
{
    /* Elements of the listOfBlocks array. Each stores a marker of whether the 
     * block is free, and two "pointers" to other blocks which are actually just
     * int-type indices.
     * */
    class BlockInfo
    {
    public:
        bool free;
        int prev, next;

        BlockInfo();
        BlockInfo(bool, int, int);
    };

    int firstFreeBlock, lastFreeBlock, totalFreeFragments;
    bool full;
    BlockInfo **listOfBlocks; // array of pointers to BlockInfo

public:
    StorageSpace();
    ~StorageSpace();
    bool insertSpace(spaceList_t);
    spaceList_t findSpace(int bytes, int start);
    bool deleteSpace(spaceList_t);
    bool startOfFrag(int index);
    int getTotalFragments();
    bool isFull();
    void printFreeSpace();
};