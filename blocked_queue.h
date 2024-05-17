#ifndef BLOCKED_QUEUE_H
#define BLOCKED_QUEUE_H

#define QUEUE_SIZE 3

typedef struct
{
    int count;
    int blockedQueue[QUEUE_SIZE];
} blocked_queue;

void initBlockedQueue(blocked_queue *q);
void printBlockedQueue(blocked_queue *q);
void enqueueProcess(blocked_queue *q, int processID);
void dequeueProcess(blocked_queue *q, int processID);

#endif