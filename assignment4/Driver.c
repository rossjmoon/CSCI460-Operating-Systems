/******************************************************************* 
 * File:        Driver.c
 * Author:      Ross Moon
 * Date:        02/23/2016
 * Description: Main entry point for Zombie vs Human Simulation.  
 * Assumptions:
 * 		1) Time required to pass a friendly on the path is negligible.
 *		2) Path control remains with current owners until the current
 *			owner's population is equal to the opponent's population.
 *		3) In the case of an equal population, neither side advance
 *			 on path.  Everyone pauses in fear of pending conflict.
 *		3) Path is cleared instantaneously on control swap.  Humans 
 *			teleport to safety, and zombies are summoned home by 
 *			their necromancer friend.  Zombies and humans are very 
 *			afraid of conflict.
 *		4) Zombies and humans that leave the path are put at the back
 *			of the line at their starting location.  It's only fair	
 *			that everyone gets a turn to use the path.   
 ********************************************************************/

 // Includes
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include "Queue.h"


// Global Constants - all times in minutex
#define SIM_TIME 10080

#define TRAVEL_TIME 10

#define CEM_Z_AVG 15
#define CEM_Z_MIN 0
#define CEM_Z_MAX 30

#define CEM_H_AVG 35
#define CEM_H_MIN 15
#define CEM_H_MAX 85

#define TWN_Z_AVG 30
#define TWN_Z_MIN 10
#define TWN_Z_MAX 70

#define TWN_H_AVG 30
#define TWN_H_MIN 10
#define TWN_H_MAX 70

#define noControl -1
#define humanControl 0
#define zombieControl 1

#define empty -1
#define zombie 'z'
#define bigZ 'Z'
#define human 'h'
#define bigH 'H'

// Function Prototypes
void generateEvents(int min, int max, Queue *eventQ);
int getNewSpawns(int curTime, Queue *waitQueue, Queue *eventQueue);
int advancePath(int path[], Queue *waitQueue);
void resetPath(int path[], Queue *waitQueue);
void displayPath(int left[], int right[], int control);

int main(int argc, char **argv){
	// seed random number
	srand(time(NULL));
	
	// Initialize and generate Zombie spawn events at the Cemetery
	Queue cemZombieEventQ;
	initializeQueue(&cemZombieEventQ);
	generateEvents(CEM_Z_MIN, CEM_Z_MAX, &cemZombieEventQ);
	
	// Initialize and generate Human spawn events at the Cemetery
	Queue cemHumanEventQ;
	initializeQueue(&cemHumanEventQ);
	generateEvents(CEM_H_MIN, CEM_H_MAX, &cemHumanEventQ);
	
	// Initialize and generate Zombie spawn events at the Town
	Queue twnZombieEventQ;
	initializeQueue(&twnZombieEventQ);
	generateEvents(TWN_Z_MIN, TWN_Z_MAX, &twnZombieEventQ);
	
	// Initialize and generate Human spawn events at the Town
	Queue twnHumanEventQ;
	initializeQueue(&twnHumanEventQ);
	generateEvents(TWN_H_MIN, TWN_H_MAX, &twnHumanEventQ);
	
	//Initialize wait queues
	Queue czwq;					// Cemetery zombie wait queue
	initializeQueue(&czwq);
	Queue chwq;					// Cemetery human wait queue
	initializeQueue(&chwq);
	Queue tzwq;					// Town zombie wait queue
	initializeQueue(&tzwq);
	Queue thwq;					// Town human wait queue
	initializeQueue(&thwq);
	
	// Population counts
	int totalHumanPop = 0;
	int totalZombiePop = 0;
	
	// Path control variables
	int pathControl = noControl;	// default empty path
	int prevControl = noControl;	// default empty path
	
	// Initialize path
	int pathCemToTwn[TRAVEL_TIME];
	int pathTwnToCem[TRAVEL_TIME];
	int i;
	for(i = 0; i < TRAVEL_TIME; i++){
		pathCemToTwn[i] = empty;
		pathTwnToCem[i] = empty;
	}
	
	int curTime;
	for(curTime = 0; curTime <= SIM_TIME; curTime++){
		printf("time: %d\n", curTime);		
		totalZombiePop += getNewSpawns(curTime, &czwq, &cemZombieEventQ);
		totalHumanPop += getNewSpawns(curTime, &chwq, &cemHumanEventQ);
		totalZombiePop += getNewSpawns(curTime, &tzwq, &twnZombieEventQ);
		totalHumanPop += getNewSpawns(curTime, &thwq, &twnHumanEventQ);
		
		if(pathControl != noControl){
			prevControl = pathControl;
		}
		if(totalZombiePop > totalHumanPop){
			// zombies control the path
			pathControl = zombieControl;
		}
		else if(totalZombiePop < totalHumanPop){
			// humans control the path
			pathControl = humanControl;			
		}
		else if(totalZombiePop == totalHumanPop){
			// previous owners control the path
			pathControl = noControl;
		}
		else{
			// error state
		}
		if(prevControl != pathControl){
			// reset paths
			if(pathControl == humanControl){
				printf("Humans have taken control of the path!\n");
				resetPath(pathCemToTwn, &czwq);
				resetPath(pathTwnToCem, &tzwq);
			}
			else if(pathControl == zombieControl){
				printf("Zombies have taken control of the path!\n");
				resetPath(pathCemToTwn, &chwq);
				resetPath(pathTwnToCem, &thwq);
			}
			else{
				printf("No one has control of the path!\n");
			}			
		}
		printf("current Humans: %d\n", totalHumanPop);
		printf("current Zombies: %d\n", totalZombiePop);
		switch(pathControl){
		case -1:
			// no control
			printf("The path is quiet.  No one has control.\n");
			if(prevControl == humanControl){
				displayPath(pathTwnToCem, pathCemToTwn, prevControl);
			}
			else{
				displayPath(pathTwnToCem, pathCemToTwn, prevControl);				
			}
			break;
		case 0:
			// humanControl
			totalHumanPop -= advancePath(pathCemToTwn, &chwq);
			totalHumanPop -= advancePath(pathTwnToCem, &thwq);
			displayPath(pathTwnToCem, pathCemToTwn, pathControl);
			break;
		case 1:
			// zombieControl
			totalZombiePop -= advancePath(pathCemToTwn, &czwq);
			totalZombiePop -= advancePath(pathTwnToCem, &tzwq);
			displayPath(pathTwnToCem, pathCemToTwn, pathControl);
			break;
		default:
			// error state
			break;
		} // end switch
		
		sleep(1);
		printf("\033[H\033[J");  // clear screen
	} // end for( curTime )	
	
	printf("total Humans Remaining: %d\n", totalHumanPop);
	printf("total Zombies Remaining: %d\n", totalZombiePop);
	return 0;
} // end main()

void generateEvents(int min, int max, Queue *eventQ){
	
	int interval = max - min + 1;		// inclusive of min and max edges
	int time = 0;
	
	// Generate event queue
	while(time <= SIM_TIME){
		// randomly generate time interval to next spawn
		time += (rand() % interval) + min;	
		// add event to queue
		eventQ->push(eventQ, time);
	}	
} // end generateEvents()

int getNewSpawns(int curTime, Queue *waitQueue, Queue *eventQueue){
	int newSpawn = eventQueue->peek(eventQueue);
	int count = 0;
	while(newSpawn == curTime){
		count++;
		waitQueue->push(waitQueue, newSpawn);
		eventQueue->pop(eventQueue);
		newSpawn = eventQueue->peek(eventQueue);
	}
	return count;
} // end getNewSpawns()

int advancePath(int path[], Queue *waitQueue){
	int doneWithPath = 0;
	int i;
	for(i = TRAVEL_TIME - 1; i >= 0; i--){
		if(i == 0){
			if(path[i] == empty && waitQueue->peek(waitQueue) != -1){
				path[i] = waitQueue->peek(waitQueue);
				waitQueue->pop(waitQueue);
			}
		}
		else if(i == (TRAVEL_TIME - 1)){
			if(path[i] != empty){
				// someone made it to the other side!
				doneWithPath++;
				path[i] = path[i-1];
				path[i-1] = empty;
			}
			else{
				path[i] = path[i-1];
				path[i-1] = empty;
			}				
		}
		else{
			path[i] = path[i-1];
			path[i-1] = empty;
		}
	}
	return doneWithPath;
} // end advancePath

void resetPath(int path[], Queue *waitQueue){
	int i;
	for(i = 0; i < TRAVEL_TIME; i++){
		if(path[i] != empty){
			waitQueue->push(waitQueue, path[i]);
			path[i] = empty;
		}
	}
}

void displayPath(int left[], int right[], int control){
	char type, bigType;
	
	if(control == humanControl){
		type = 'h';
		bigType = 'H';
	}
	else if(control == zombieControl){
		type = 'z';
		bigType = 'Z';
	}
	else{
		type = '=';
		bigType = '=';
	}
	printf("_____          _____\n");
	printf("|TWN|");
	int i, j;
	for(i = 0; i < TRAVEL_TIME; i++){
		j = TRAVEL_TIME - 1 - i;
		if(left[i] != empty && right[j] != empty){
			printf("%c",bigType);
		}
		else if(left[i] != empty){
			printf("%c",type);
		}
		else if(right[j] != empty){
			printf("%c",type);
		}
		else{
			printf("=");
		}
	}
	printf("|CEM|\n");
	printf("-----          -----\n");			
}


