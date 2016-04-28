/**
*  file: 	prog1.c
*  author:	Ross Moon
*  Description: Assignment 3	
*/

// Include for printf()
#include <stdio.h>
// Include for pid_t data type
#include <sys/types.h>
// Include for exit()
#include <stdlib.h>
// Include for open()
#include <fcntl.h>
#include <sys/stat.h>
// Include for read()
#include <unistd.h>

// Constant variable
#define MAX_READ 256

// Function prototypes
void getString(char* inputString);

void reverseString(char* inputString, int length);

void capitalizeString(char* inputString, int length);

int length(char* inputString);


// Global Constants
#define PIPE1 "FIFO1"
#define PIPE2 "FIFO2"
#define PIPE3 "FIFO3"
#define BUF_SIZE 256

int main(int argc, char *argv[]){
      
    pid_t child1 = fork();  
    if(child1 != 0){
        // parent process
        // prompt user for string
        
        int pipe1Open, pipe3Open, fileWrite, fileRead;
    
        char inputString[BUF_SIZE];
        
        // Create PIPE1 
        if(mkfifo(PIPE1, 0666) != 0){
            // mkfifo() error
            printf("ERROR: Failed to create PIPE1 file.");
            return -1;
        }
        
        getString(inputString);
        // write to PIPE1
        if((pipe1Open = open(PIPE1, O_RDWR)) > 0){
            if((fileWrite = write(pipe1Open, &inputString, BUF_SIZE)) < 0){
                // write error
                printf("ERROR: Failed to write to fifo in parent");
                return -1;
            }
        }
        else{
            printf("ERROR: Failed to open PIPE1");
            return -1;
        }
       
        

        // retrieve solution
        char solution[BUF_SIZE];
        if((pipe3Open = open(PIPE3,O_RDWR)) > 0){
            if((fileRead = read(pipe3Open, &solution, BUF_SIZE)) < 0){
                // read error
                fprintf(stderr, "ERROR: Failed to open PIPE3 in parent");
                return -1;
            }
            printf("Porcessed string is: %s", solution);
        }
        // wait for write to PIPE3 
        wait(0);
        close(pipe1Open);
        
    }
    else {

        //child1's process
        // reverse the string       
        //wait(0);
        pid_t child2 = fork();  
        if(child2 != 0){
            // child2's parent process
                 
            // Create PIPE2
            if(mkfifo(PIPE2, 0666) != 0){
                // mkfifo() error
                printf("ERROR: Failed to create PIPE2 file.");
                return -1;
            }
            // Child1 variables
            int pipe1Open, pipe2Open, fileWrite, fileRead;
            char revStr[BUF_SIZE];
            // open and read PIPE1 
            pipe1Open = open(PIPE1, O_RDWR);
            if(pipe1Open > 0){
                if((fileRead = read(pipe1Open, &revStr, BUF_SIZE)) < 0){
                    // read error
                    fprintf(stderr, "ERROR: Failed to read PIPE1 in child1");
                    return -1;
                }
            }
            // reverse string
            int length = getLength(revStr);
            reverseString(revStr, length);
            // write reverse string to PIPE2
            if((pipe2Open = open(PIPE2, O_RDWR)) > 0){ 
                if((fileWrite = write(pipe2Open, &revStr, BUF_SIZE)) < 0){
                    // write error
                    printf("ERROR: Failed to write to fifo in parent");
                    return -1;
                }        
            }
            // close pipes 1 and 2
            close(pipe1Open);
            close(pipe2Open);
        }
        else{
            // child2's process - capitalize string
            if(mkfifo(PIPE3, 0666) != 0){
                
                // mkfifo() error
                printf("ERROR: Failed to create PIPE3 file.");
                return -1;
            }
            // Child varibales
            int pipe2Open, pipe3Open, fileWrite, fileRead;
            char capStr[BUF_SIZE];
            // read PIPE2
            if((pipe2Open = open(PIPE2, O_RDWR)) > 0){         
                if((fileRead = read(pipe2Open, &capStr, BUF_SIZE)) < 0){
                    printf("ERROR: Failed to read PIPE2 in child2");
                    return -1;
                } 
            }
                        
            // capitalize string
            int length = getLength(capStr); 
            capitalizeString(capStr, length);
            
            // Open PIPE3 and write capitalized string
            if((pipe3Open = open(PIPE3, O_RDWR)) > 0){
            
                // write capitalized string to PIPE3
                if(fileWrite = (write(pipe3Open, &capStr, BUF_SIZE)) < 0){
                    // write error
                    printf("ERROR: Failed to write to PIPE3 in child2");
                    return -1;
                }
            }   
            // exit child2
            exit(0);
        }
        // exit child1
        exit(0);
    }
    // remove all pipes
    unlink(PIPE1);
    unlink(PIPE2);
    unlink(PIPE3);
    return(0);
}



/*
 * getString - propmts I/O for a string
 * Input:  inputString: char in/out
 * */ 
void getString(char* inputString){
       
    printf("Please enter a string: ");
    fgets(inputString, MAX_READ, stdin);   
}

/*
 * reverseString - reverses the order of a string
 * Inputs:  inputString:    char in/out
 *          length:         int in/-
 */
void reverseString(char* inputString, int length){
   
    char copy[256];
    unsigned int i;
    // create deep copy of inputString
    for(i = 0; i < length; i++){
        copy[i] = inputString[i];        
    }
    // reverse the order of characters
    for(i = 0; i < length; i++){
     
        inputString[i]= copy[length-1-i]; 
    }
}

/*
 * capitalizeString - convert lower case alphabet characters 
 * to uppercase alphabet characters
 * Inputs:  inputString:    char in/out
 *          length:         int in/-
 */
void capitalizeString(char* inputString, int length){
  
    char test;
    unsigned int i;
    // Iterate through inputString
    for(i = 0; i < length; i++){
        test = inputString[i];
        // check for lower case character
        if(test >= 'a' && test <= 'z'){
            // set to equivalent upper case character
            inputString[i] = ('A' + test - 'a');
        }
    }   
}

int getLength(char* inputString){

    int length = 0;
    // while not at end of string
    while(inputString[length] != '\n'){
        // increment count
        length++;
    }
    return length;
}

