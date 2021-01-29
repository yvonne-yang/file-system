/*
 * Yvonne Yang
 * Jan 24-28, 2021
 * 
 * A simplified file system model that supports 3 operations, 
 * save, delete and read. 
 * 
 * Please read the ReadMe.md.
 * 
 * To compile:
 * g++ -Wall main.cpp StorageSpace.cpp FileLookUp.cpp -o file-system.exe
 */
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <unordered_map>
#include <cmath>

#include "StorageSpace.h"
#include "FileLookUp.h"
#include "globals.h"

/* Global variables */
static StorageSpace storageSpace;
static FileLookUp fileLookUp;

/* Functions */
bool saveFile(std::string fileID, int bytes);
bool deleteFile(std::string fileID);
bool readFile(std::string fileID);
void help();

/* 
 * Prints a list of available spaces in the storage, and asks the user where  
 * they would like to store the file. The file with ID [fileID] that has [size]
 * number of bytes will then be saved into the file system.
 * 
 * @return True if error--there is not enough memory.
 * */
bool saveFile(std::string fileID, int bytes)
{
    if (storageSpace.isFull()) // check if storage is full
    {
        std::cout << "Error. Storage is full." << std::endl;
        return true;
    }
    if (fileLookUp.contains(fileID)) // is the fileID valid?
    {
        std::cout << "File already exists." << std::endl;
        return true;
    }

    /* Step 1: print list of available spaces */
    std::cout << "Current free memory blocks: " << std::endl;
    storageSpace.printFreeSpace();

    /* Step 2: ask for input on the interval of choice */
    std::cout << "Enter the starting index of a free fragment that you would like to write to (eg. 27 for 27..45):" << std::endl;
    std::cout << "> ";
    // basic input validation
    int start = 0;
    std::cin >> start;
    if (std::cin.fail()) // invalid input type
    {
        std::cout << "Invalid input." << std::endl;
        std::cin.clear();
        std::cin.ignore(INT_MAX, '\n');
        return false;
    }
    /****** validate the chosen index. Note that a less safe option is the isFree(int) method in StorageSpace ********/
    else if (!storageSpace.startOfFrag(start))
    {
        std::cout << "Invalid starting index." << std::endl;
        std::cin.clear();
        std::cin.ignore(INT_MAX, '\n');
        return false;
    }
    // is there enough memory?
    int blocksRequired = ceil((double)bytes / 1024.0 / BLOCK_SIZE);
    if (blocksRequired > storageSpace.getTotalFreeBlocks())
    {
        std::cout << "File size too big." << std::endl;
        return true;
    }

    /* Step 3: save the file */
    auto listOfSpaces = storageSpace.findSpace(blocksRequired, start);
    // save file in fileLookUp
    fileLookUp.saveFile(fileID, listOfSpaces);
    // update the free storage
    storageSpace.occupySpace(listOfSpaces); // mark those spaces as occupied

    std::cout << "File successfully saved." << std::endl;
    return false;
}

bool deleteFile(std::string fileID)
{
    // delete from fileLookUp
    auto fragList = fileLookUp.deleteFile(fileID);
    // free up space in storage
    storageSpace.freeUpSpace(fragList);
    std::cout << "File successfully deleted." << std::endl;
    return false;
}

bool readFile(std::string fileID)
{
    std::cout << "Printing the memory blocks that store \"" << fileID
              << "\"..." << std::endl;
    fileLookUp.printFileAddress(fileID);
    return false;
}

/* Extra feature: let user set storage 
bool set(int storage, int block)
{
}
*/

int main()
{
    std::cout << "Welcome to Yvonne's file system. " << std::endl;
    std::cout << std::endl;
    help();

    /* Take user input */
    std::string line, cmd;
    while (true)
    {
        std::cout << "> ";
        std::getline(std::cin, line);
        std::stringstream lineStream(line);
        if (lineStream.peek() == '\n' || lineStream.eof())
        {
            continue;
        }

        lineStream >> cmd;

        /* Process user input and check if the command is valid */
        if (cmd == "exit")
        {
            std::cout << "Thank you for using Yvonne's file system." << std::endl;
            return 0;
        }
        else if (cmd == "save")
        {
            std::string fileID;
            int bytes;
            lineStream >> fileID;
            lineStream >> bytes;

            if (lineStream.fail()) // input validation
                std::cout << "Invalid arguments. Usage: save [fileID] [size]." << std::endl;
            else
                saveFile(fileID, bytes);
        }
        else if (cmd == "delete")
        {
            std::string fileID;
            lineStream >> fileID;

            if (lineStream.fail()) // input validation
                std::cout << "Invalid arguments. Usage: delete [fileID]" << std::endl;
            else
                deleteFile(fileID);
        }
        else if (cmd == "read")
        {
            std::string fileID;
            lineStream >> fileID;

            if (lineStream.fail()) // input validation
                std::cout << "Invalid arguments. Usage: read [fileID]" << std::endl;
            else
                readFile(fileID);
        }
        else if (cmd == "help")
            help();
        else
        {
            if (!std::cin.eof()) // input validation
                std::cout << "Error. Command not found. Enter \"help\" to see the help menu." << std::endl;
        }
    }

    return 0;
}

/* Helper function to print the list of possible commands.*/
void help()
{
    std::cout << "Below are the possible commands.\nEnter \"help\" to see this menu again." << std::endl;
    std::cout << std::endl;
    std::cout << std::setw(25) << std::left << "save [fileID] [size]";
    std::cout << "Saves the file with ID [fileID] and [size] in bytes in the file system. " << std::endl;
    std::cout << std::setw(25) << std::left << "delete [fileID]";
    std::cout << "Deletes the file with ID [fileID]." << std::endl;
    std::cout << std::setw(25) << std::left << "read [fileID]";
    std::cout << "Prints the blocks of memory taken up by the file with ID [fileID]." << std::endl;
    /* Extra feature
    std::cout << std::setw(25) << std::left << "set [storage] [block_size]";
    std::cout << "Erases all current files and sets the storage size in MiB and the size of each block of memory in KiB. The default values are 1MiB and 1KiB." << std::endl;
    */
    std::cout << std::setw(25) << std::left << "exit";
    std::cout << "Exits the program." << std::endl;
}