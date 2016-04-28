/*
* File: fc.c
* author:  Ross Moon
* date: 01/28/2016
* Description: A rewrite of the file copy method.  Use the interactive input/output of the CLI to call fc.c
*/

// Includes for open()
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
// include for printf()
#include <stdio.h>
// include for read()
#include <unistd.h>
// include for boolean
#include <stdbool.h>

// constant buffer size
#define BUF_SIZE 8

int main(int argc, char *argv[]){
	// variable declarations
	char src[256];	// input filename from user I/O.  source of copy
	char dest[256];	// output filename from user I/O.  destination of copy
	int readBytes;	// bytes read from source
	int writeBytes;	// bytes written to destination
	int srcFile;	// handle of source file
	int destFile;	// handle of destination file
	unsigned char buffer[BUF_SIZE];	// copy buffer
	bool goodInput = false;

	while(!goodInput){
		// get user input
		printf("Please enter source:  ");
		scanf("%s", &src);
		printf("Please enter destination:  ");
		scanf("%s", &dest);
	    
        // open source file
   		srcFile = open(src, O_RDONLY);
    	if(srcFile > 0){
            // open dest file
    	    destFile = open(dest, O_WRONLY | O_CREAT);
            if(destFile ==  -1){
                // failed to open destination file. prompt user again
                close(srcFile);
                printf("Unable to open destination file: %s", dest);
            }
            else{
                // we have good user input
                goodInput = true;
            }
        }
        else{
            // failed to open source file.  prompt user again
            printf("Unable to open source file: %s", src);
        }    
    }// end while

	// preform read->write from src to dest files
	while((readBytes = read(srcFile, &buffer, BUF_SIZE)) > 0){
		writeBytes = write(destFile, &buffer, BUF_SIZE);
		if(readBytes != writeBytes){
			// copy error
			printf("ERROR: copy failed.\n");
			return -1;
		}// end if
	}// end while

    // copy was successfull
	// close files and exit applicaiton
	close(srcFile);
	close(destFile);
	printf("Copy successful.\n");
    return 0;
}// end main
