#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdbool.h>

#define QUEUE_SIZE 3

typedef struct
{
    int pid;
    int priority;
} queue_element;

typedef struct
{
    int count;
    queue_element elements[QUEUE_SIZE];
} priority_queue;

void initPQueue(priority_queue *q);
void printPQueue(priority_queue *q);
void enqueueP(priority_queue *q, int processID, int priority);
int dequeueP(priority_queue *q);
bool isEmptyP(priority_queue *q);

#endif