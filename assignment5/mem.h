/*********************************************************************
 * File:        mem.h
 * Author:      Ross Moon
 * Date:		02/23/2016
 * Description: Memory management.  Allocate and free memory.
 ********************************************************************/

#ifndef MEM_H
#define MEM_h

#include <stdio.h>

typedef struct MemMap{
    int size;	// memory size
	int addr;	// address of memory allocation
} mmap, *mptr;

/* init()
 * Initialize tables for allocated and free memory.
 */
void init();

/* mymalloc() 
 * Allocate size units from the given map.
 * Return the base of the allocated space.
 * Uses best fit algorithm
 * @param: size - size of memory to be allocated
 * @return: baseAddr - base address of memory allocation
 */ 
int mymalloc(int size);

/* mymfree() 
 * Free the prviously allocated space
 * of size units into the specified map.
 */  
void mymfree();

/* mymdump()
 * Prints to console the current avalable 
 * memory.
 */ 
void mymdump();

#endif
