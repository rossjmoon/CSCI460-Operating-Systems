/******************************************************************* 
 * File:        Queue.c
 * Author:      Ross Moon
 * Date:		02/23/2016
 * Description: Node based implementation of a Queue data structure
 ********************************************************************/ 

#include "Queue.h"


void initializeQueue(Queue *q){
	
	q->head = NULL;
	q->tail = NULL;
	q->size = 0;
	q->push = push;
	q->pop = pop;
	q->peek = peek;
	q->print = print;
	
} // end initializeQueue()
void print(Queue *q){
    printf("printing: ");
	Node* traverse = (Node*) malloc(sizeof(Node));
    traverse = q->head;
    while(traverse != NULL){
        printf("%d ",traverse->time);
        traverse = traverse->next;
    }
	printf("\n");
} // end print()

void push(Queue *q, int newTime){
    	
    Node* newNode = (Node*) malloc(sizeof(Node));
    newNode->time = newTime;
	newNode->next = NULL;
    if(q->size == 0){
		q->head = newNode;
		q->tail = newNode;
	}
	else{
		q->tail->next = newNode;
	}
	q->tail = newNode;
	q->size++;
} // end push()

void pop(Queue *q){
    
	if(q->size > 0){
		q->size--;
		Node* nodeToDelete = (Node*) malloc(sizeof(Node));
		nodeToDelete = q->head;
		
		// advance head pointer
		q->head = q->head->next;
		
		free(nodeToDelete);
		nodeToDelete = NULL;				
	}	
} // end pop()


int peek(Queue *q){
    if(q->size == 0){
		return -1;		
	}
	else{
		return q->head->time;
	}    
} // end peek()
