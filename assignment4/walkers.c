/********************************************************************
 * File:        walkers.c
 * Author:      Ross Moon
 * Description: Process that spawns zombies.
 ********************************************************************/

/** Includes **/
#include <semaphore.h>
#include <stdio.h>
#include <errno.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
// Random number generation
#include <stdlib.h>
#include <time.h>

#define SIM_TIME 10080 // in minutes

/* Function Prototypes */
int spawnZombie(int dest);
void generateZombieQueue(Queue zombieQ);


int main(int argc, char **argv){
   
    // Init random number generator
    time_t t;
    srand(time(&t));
    
    // Init for loop vars
    int i;
    double total = 0;
    for(i = 0; i < SIM_TIME; i++){
        total += generateZombie(0);
//        fprintf(stderr, "total = %f\n", total);
    }
    double temp = total / SIM_TIME;
    fprintf(stderr, "Avg: %f\n", temp);

    
    return 0;
}

int spawnZombie(int dest){
    
    int spawnChance;
    if(dest == 0){
        // cemetary destination
        spawnChance = 20;
    }
    else if(dest == 1){
        // town destination
        spawnChance = 10;
    }
    else{
        // error: invalid input
        return -1;
    }
    
    int randNum = rand() % 100;
//    fprintf(stderr, "rand = %d\n", randNum);
    if(randNum < spawnChance){
//        fprintf(stderr, "return 1\n");
        return 1;
    }
    return 0;
}

void generateZombieQueue(Queue zombieQ){

}


