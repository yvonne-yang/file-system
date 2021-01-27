/*
 * Yvonne Yang
 * Jan 24, 2021
 * 
 * A simplified file system model that supports 3 operations, 
 * save, delete and read. See ReadMe.md for details.
 * 
 * To compile:
 * g++ -Wall main.cpp StorageSpace.cpp -o file-system.exe
 */
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <unordered_map>

#include "StorageSpace.h"
#include "globals.h"

/* Global variables */
std::unordered_map<std::string, std::vector<std::pair<int, int>>> fileLoopUp;

/* Functions */
bool saveFile(std::string fileID, int bytes);
bool deleteFile(std::string fileID);
bool readFile(std::string fileID);
void help();

/* 
 * Prints a list of available spaces in the storage, and asks the user where  
 * they would like to store the file. The file with ID [fileID] that has [size]
 * number of bytes will then be saved into the file system.
 * */
bool saveFile(std::string fileID, int bytes)
{
    std::cout << "File successfully saved." << std::endl;
    return false;
}

bool deleteFile(std::string fileID)
{
    std::cout << "File successfully deleted." << std::endl;
    return false;
}

bool readFile(std::string fileID)
{
    std::cout << "End of file." << std::endl;
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

            if (lineStream.fail())
                std::cout << "Invalid arguments. Usage: save [fileID] [size]." << std::endl;
            else
                saveFile(fileID, bytes);
        }
        else if (cmd == "delete")
        {
            std::string fileID;
            lineStream >> fileID;

            if (lineStream.fail())
                std::cout << "Invalid arguments. Usage: delete [fileID]" << std::endl;
            else
                deleteFile(fileID);
        }
        else if (cmd == "read")
        {
            std::string fileID;
            lineStream >> fileID;

            if (lineStream.fail())
                std::cout << "Invalid arguments. Usage: read [fileID]" << std::endl;
            else
                readFile(fileID);
        }
        else if (cmd == "help")
            help();
        else
        {
            if (!std::cin.eof())
                std::cout << "Error. Command not found. Enter \"help\" to see the help menu." << std::endl;
        }
    }

    return 0;
}

/* Helper function to print the list of possible commands.*/
void help()
{
    std::cout << "Below are the possible commands.\n Enter \"help\" to see this menu again." << std::endl;
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