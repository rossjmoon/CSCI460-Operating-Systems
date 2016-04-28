/*********************************************************************
 * File:        Queue.h
 * Author:      Ross Moon
 * Date:		02/23/2016
 * Description: Node based implementation of a Queue data structure
 ********************************************************************/

#ifndef QUEUE_H
#define QUEUE_h

#include <stdio.h>
#include <stdlib.h>



typedef struct NodeTag{
    int time;
	struct NodeTag *next;
} Node;


typedef struct QueueTag{
    Node *head;
    Node *tail;
    int *size;
    void (*push) (struct QueueTag*, int);
    void (*pop) (struct QueueTag*);
    int (*peek) (struct QueueTag*);
    void (*print) (struct QueueTag*);
} Queue, *qptr;

void initializeQueue(Queue *q);
void push(Queue* q, int newTime);
void pop(Queue* q);
int peek(Queue* q);
void print(Queue* q);
#endif
