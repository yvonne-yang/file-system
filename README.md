# Yvonne's Simple File System
---
A simple file system model that supports three operations, read, save and delete.
Given the storage size (MiB), X, and the size of each block of memory (KiB), Y, as inputs, all three operations run in constant time, ie. O(1).  

(As of now, the storage size is set to 1MiB and the block size is 1KiB.)  

This implementation uses a hash table for file address lookup and a custom made data structure to keep track of storage space. The problem can be though of as two subproblems: managing free space and managing files. Thus the two data structures.  

The FileLookUp class is basically a good ol' hash table. This covers the O(1) for the read operation and part of the save operation.  

The StorageSpace class is an ingenious creation tailored to solve this problem. In essence, to complete the save and delete functions, we need a fixed-size data structure that is partitioned into fragments, and supports fast concatenation,splitting, and find. Linked lists seem like a good option for concatenation and splitting of intervals, however, an array is much better at find. The heart and soul of this data structure is the realization that the linked list and array can function as one structure. This leverages the fact that a storage memory does not change often.  

 

## To run
Download the source code and use `g++ -Wall main.cpp StorageSpace.cpp FileLookUp.cpp -o file-system.exe` to generate the executable.
Run the executable and use the console for input. Follow instructions in the help menu.


## Features that can be added
- Ask user to set X and Y
- Easier input for choosing a free fragment
- Frontend
