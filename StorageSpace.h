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

#include <tuple>
#include <array>
#include "StorageFragment.h"
#include "globals.h"

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
    bool insertSpace(int start, int end);
    bool deleteSpace(int start, int end);
    bool isFull();
    void printFreeSpace();
};