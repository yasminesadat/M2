#ifndef QUEUE_H
#define QUEUE_H

#define QUEUE_SIZE 3

typedef struct
{
    int count;
    int Elements[QUEUE_SIZE];
} queue;

void enqueue(queue *q, int processID);
int dequeue(queue *q);
bool isEmpty(queue *q);

#endif