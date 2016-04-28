/******************************************************************* 
 * File:        mem.c
 * Author:      Ross Moon
 * Date:        03/23/2016
 * Description: Implementation of malloc and free using a Best Fit
 *				algorithm.  
 ********************************************************************/

 
#include "mem.h"

#define MAX_MEM 1024 

mmap freeTable[MAX_MEM];
mmap memTable[MAX_MEM];
int initMem = 0;	// 0 - memory not initialized.  1 - memory has been initialized
int freeTableSize = 0;
int memTableSize = 0;

void init(){
	// initialize free table
	freeTable[0].addr = 1;
	freeTable[0].size = MAX_MEM;
	freeTableSize++;
	
	// initialize memory table
	memTable[0].addr = 1;
	memTable[0].size = 0;
	memTableSize++;
	
	// flag initializtion
	initMem = 1;
} // end init()


int mymalloc(int size){
	if(initMem == 0){
		init();
	}
	// initialize method variables
	int baseAddr;
	int bestSize = MAX_MEM;
	int bestFit;
	
	// search free memory table for best fit
	int i;
	for(i = 0; i < freeTableSize;i++){
		if(freeTable[i].size >= size){
			// possible fit found
			int diff = size - freeTable[i].size;
			if(diff == 0){
				// best fit found
				bestFit = i;
				break;
			}
			else if(diff < bestSize){
				// update best fit
				bestSize = diff;
				bestFit = i;
			}	
		}
	} // end for
	
	// Best Fit address found
	baseAddr = freeTable[bestFit].addr;
	
	// remove free memory from freeTable
	freeTable[bestFit].addr += size;
	freeTable[bestFit].size -= size;
	
	if(freeTable[bestFit].size == 0){
		// section of memory is empty.  remove entry from table
		for(i = 0; i < freeTableSize; i++){
			if(i == freeTableSize-1){
				// end of array, set to 0
				freeTable[i].size = 0;
				freeTable[i].addr = 0;
			}
			else{
				// shift array left
				freeTable[i] = freeTable[i + 1];
			}
		} // end for
	}
	
	// add new memory allocation to memTable
	memTable[memTableSize-1].addr = baseAddr;
	memTable[memTableSize-1].size += size;
	memTableSize++;
	
	return baseAddr;
} // end mymalloc()

void mymfree(int addr){
	if(initMem == 0){
		init();
	}
	
	// search memTable for address
	int i, j;
	int memFreed = 0; 	// 0 - no memory freed, 1 - memory freed
	int size = 0;	// size of of memory to be freed
	for(i = 0; i < memTableSize; i++){
		if(memTable[i].addr == addr){
			// memory address to free found
			size = memTable[i].size;
			for(j = i; j < memTableSize; j++){
				if(j == memTableSize-1){
					// end of array, set to 0
					memTable[j].size = 0;
					memTable[j].addr = 0;
				}
				else{
					// shift array left
					memTable[j] = memTable[j + 1];
				}				
			} // end for
			memTableSize--;
			memFreed = 1;	// flag memory was freed
			break;
		}
	} // end for
	
	// coalesce freeTable
	if(memFreed == 1){
		int preIndex = -1;	// adjacent memory before (left) the newly freed memory
		int postIndex = -1; 	// adjacent memory after (right) the newly freed memory
		for(i = 0; i < freeTableSize; i++){
			if((freeTable[i].addr + freeTable[i].size) == addr){
				// left adjacent free memory found
				preIndex = i;
			}
			else if(freeTable[i].addr == (addr + size)){
				// right adjacent free memory found
				postIndex = i;
			}
		}		
		if(preIndex >= 0 && postIndex >= 0){
			// free memory adjacent before and after newly freed memory
			freeTable[preIndex].size += (size + freeTable[postIndex].size);
			for(i = postIndex; i < freeTableSize; i++){
				if(i == freeTableSize-1){
					// end of array, set to 0
					freeTable[i].size = 0;
					freeTable[i].addr = 0;
				}
				else{
					// shift array left
					freeTable[i] = freeTable[i + 1];
				}
			} // end for
			freeTableSize--;
		}
		else if(preIndex >= 0){
			// free memory only adjacent before
			freeTable[preIndex].size += size;
		}	
		else if(postIndex >= 0){
			// free memory only adjacent after
			freeTable[postIndex].addr -= size;
			freeTable[postIndex].size += size;
		}
		else{
			// no adjacent free memory
			freeTableSize++;
			freeTable[freeTableSize-1].addr = addr;
			freeTable[freeTableSize-1].size = size;
		}
	} // if memFreed	
} // end mymfree()


void mymdump(){
	if(initMem == 0){
		init();
	}
	printf("---------Free Memory Table---------\n");
	int i;
	for(i = 0; i < freeTableSize; i++){
		printf("Address: %8d    Size: %8d\n", freeTable[i].addr, freeTable[i].size);
	}
	printf("\n");
} // end mymdump()
