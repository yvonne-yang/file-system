/* 
 * Yvonne Yang
 * 01/26/21
 * 
 * Global macros and constants.
 *  
 * */

#pragma once

#define NULLI -1 // define a negative number to be the "nullpointer" value since valid indices are all positive

const int STORAGE = 1024; // in KiB
const int BLOCK_SIZE = 1; // in KiB
const int NUM_BLOCKS = STORAGE / BLOCK_SIZE;