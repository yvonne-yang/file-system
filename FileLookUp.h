/*
 * Yvonne Yang
 * 01/28/2021
 * 
 * 
 * typedefs to make code more readable.
 * 
 * The fileLookUp stores information about each file in a hash table for
 * easy lookup. The key of each pair of file info is the fileID, and the value
 * is a series of memory fragments that store the file. 
 * 
 * Basically, a FileLookUp is of type 
 * std::unordered_map<std::string, std::vector<std::pair<int, int>>>
 * */

#include <unordered_map>
#include <vector>

typedef std::pair<int, int> fragment_t;     // a start and an end index define
                                            // a storage fragment
typedef std::vector<fragment_t> fragList_t; // fragment list
typedef std::string fileID_t;

class FileLookUp
{
    std::unordered_map<fileID_t, std::vector<fragment_t>> table;
    int totalFiles;

public:
    FileLookUp();
    bool contains(fileID_t fileID);
    fragList_t findFile(fileID_t fileID);
    void saveFile(fileID_t fileID, fragList_t);
    void deleteFile(fileID_t fileID);
    void printFileAddress(fileID_t fileID);
};