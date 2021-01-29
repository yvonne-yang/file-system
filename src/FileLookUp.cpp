/*
 * Yvonne Yang
 * 01/28/2021
 * 
 * Implementation file.
 * The FileLookUp stores information about each file in a hash table for
 * easy lookup. The key of each pair of file info is the fileID, and the value
 * is a series of memory fragments that store the file. 
 * */
#include <vector>
#include <utility>
#include <iostream>

#include "FileLookUp.h"

FileLookUp::FileLookUp()
{
    totalFiles = 0;
}

/* @return True if a file with that id already exists. */
bool FileLookUp::contains(fileID_t fileID)
{
    return (table.find(fileID) != table.end());
}

/* Finds the address of a file with id [fileID].
 * @return The list of fragments where the file is stored */
fragList_t FileLookUp::findFile(fileID_t fileID)
{
    auto file = table.find(fileID);
    if (file == table.end())
    {
        std::cout << "Error. File not found." << std::endl;
        return {};
    }
    return file->second;
}

/* Logs a file into the lookup table */
void FileLookUp::saveFile(fileID_t fileID, fragList_t fragments)
{
    std::pair<fileID_t, fragList_t> file(fileID, fragments);
    table.insert(file);
    totalFiles++;
}

/* Deletes a file from the lookup table */
fragList_t FileLookUp::deleteFile(fileID_t fileID)
{
    auto file = table.find(fileID);
    auto fragList = file->second;
    table.erase(file);
    totalFiles--;
    return fragList;
}
/* Prints the addresses of a file. */
void FileLookUp::printFileAddress(fileID_t fileID)
{
    auto fragList = findFile(fileID);
    if (fragList.empty()) // file not found error
        return;
    for (auto frag : fragList)
    {
        std::cout << frag.first << ".." << frag.second << std::endl;
    }
}