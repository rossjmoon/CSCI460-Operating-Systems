/******************************************************************* 
 * File:        Driver.c
 * Author:      Ross Moon
 * Date:        03/23/2016
 * Description: Tests methods from mem.c
 ********************************************************************/

#include <stdio.h>
#include "mem.h"

int main(int argc, char **argv){
	int baseAddr;
	
	mymdump();
	
	printf("malloc(255)\n");
	baseAddr = mymalloc(255);
	printf("baseAddr from malloc(255): %d\n", baseAddr);
	mymdump();
	
	printf("malloc(64)\n");
	baseAddr = mymalloc(64);
	printf("baseAddr from malloc(64): %d\n", baseAddr);
	mymdump();
	
	printf("free(1)\n");
	mymfree(1);
	mymdump();
	
	printf("free(256)\n");
	mymfree(256);
	mymdump();
	return 0;
}