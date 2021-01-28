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

#include "FileLookUp.h"

FileLookUp::FileLookUp()
{
    totalFiles = 0;
}

std::vector<fragment_t> FileLookUp::findFile(fileID_t fileID)
{
    auto file = table.find(fileID);
    return file->second;
}

void FileLookUp::saveFile(fileID_t fileID, std::vector<fragment_t> fragments)
{
    std::pair<fileID_t, std::vector<fragment_t>> file(fileID, fragments);
    table.insert(file);
}