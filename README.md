# Yvonne's Simple File System
---
A simple file system model that supports three operations, read, save and delete.
Given the storage size (MiB), X, and the size of each block of memory (KiB), Y, as inputs, all three operations run in constant time, ie. O(1).  

(As of now, the default storage size is set to 1MiB and the block size is 1KiB.)  

## Explanation of Algorithm and Data structures
This implementation uses a hash table for file address lookup and a custom made data structure to keep track of storage space. The problem really has two subproblems: managing free space and managing files. Thus the two data structures.  

The FileLookUp class is basically a good ol' hash table. The keys are file IDs and the values are their addresses. This explains the O(1) for the read operation.  

The StorageSpace class is an ingenious creation tailored to solve this problem. In essence, to complete the save and delete functions, we need a fixed-size data structure that is partitioned into fragments, and supports fast concatenation,splitting, and find. Linked lists seem like a good option for concatenation and splitting of intervals, however, an array is much better at find. The heart and soul of this data structure is the realization that the linked list and array can function as one structure. This leverages the fact that a storage memory does not change often.  

Perhaps an example would be clearer. 

The number of memory blocks is X/Y. This is also the size of our linked-array-list. Each block can be represented by 3 things, a link to the previous node, a link to the next node, and whether it is free (F). We write this triple as (prev, free, next). As the program initializes, the storage linked-array-list looks like:
|index|0|1|2|3...1022|1023|
|-|-|-|-|-|-|
|info|(,F,1023)|(,,)|(,,)|(,,)...(,,)|(0,F,)|

Note how we only store information in the first and last block. More on this later. We will abbreviate elements that are "null" to save space.  

Let's store a file "f1" that takes up 20 blocks. To avoid fragmentation, the program will only allow you to store it at the front of the storage. Now we have:
|index|0|1..18|19|20..1022|1023|
|-|-|-|-|-|-|
|info|(,,19)|(,,)..(,,)|(0,,)|(,F,1023)|(,,)..(,,)|(20,F,)|

Define a fragment as a contiguous part of the storage that belong
to a single file, or is empty. The fragment that belongs to "f1" forms a linked list. The free spaces also always form a linked list. But note that only the start and end of each fragment need to be marked, which saves a lot of time.

Let's store one more file. "f2" takes up 1 block.
|index|0|1..18|19|20|21..1022|1023|
|-|-|-|-|-|-|-|
|info|(,,19)|(,,)..(,,)|(0,,)|(20,,20)|(,F,1023)|(,,)..(,,)|(21,F,)|

Now we wish to delete "f1". Note how we only have to change information in three links. Also observe how all free fragments are chained into one list so that we can easily print all free spaces.
|index|0|1..18|19|20|21..1022|1023|
|-|-|-|-|-|-|-|
|info|(,F,19)|(,,)..(,,)|(0,F,21)|(20,,20)|(19,F,1023)|(,,)..(,,)|(21,F,)|

If we want to store "f3" which takes up 40 blocks at index 0, fragmentation is now unavoidable. Still, we only have to modify a few pointers to reflect the new storage status.  
|index|0|1..18|19|20|21|22...40|41|42|43..1022|1023|
|-|-|-|-|-|-|-|-|-|-|-|
|info|(,,19)|(,,)..(,,)|(0,,)|(20,,20)|(,,41)|(,,)..(,,)|(21,,)|(,F,1023)|(,,)..(,,)|(42,F,)|
|notes|f3|f3|f3|f2|f3|f3|f3|free|free|free|

## To run
If you'd like to compile the project and see it work, download the source code and use `[cpp-compiler] main.cpp StorageSpace.cpp FileLookUp.cpp -o file-system.exe` to generate the executable.
Run the executable and use the console for input. Follow instructions in the help menu.

## Example session
Use the "example.in" as input.
```cmd
Welcome to Yvonne's file system.

Below are the possible commands.
Enter "help" to see this menu again.

save [fileID] [size]     Saves the file with ID [fileID] and [size] in bytes in the file system.
delete [fileID]          Deletes the file with ID [fileID].
read [fileID]            Prints the blocks of memory taken up by the file with ID [fileID].
exit                     Exits the program.
> save f1 20480
Current free memory blocks:
0..1023
Enter the starting index of a free fragment that you would like to write to (eg. 27 for 27..45):
> 0
File successfully saved.
> > save f2 1
Current free memory blocks:
20..1023
Enter the starting index of a free fragment that you would like to write to (eg. 27 for 27..45):
> 20
File successfully saved.
> > read f1
Printing the memory blocks that store "f1"...
0..19
> read f2
Printing the memory blocks that store "f2"...
20..20
> delete f1
File successfully deleted.
> save f3 409600
Current free memory blocks:
0..19
21..1023
Enter the starting index of a free fragment that you would like to write to (eg. 27 for 27..45):
> 0
File successfully saved.
> > read f3
Printing the memory blocks that store "f3"...
0..19
21..400
> exit
Thank you for using Yvonne's file system.
```

## Features that can be added
- Ask user to set X and Y
- Easier input for choosing a free fragment
- Frontend
