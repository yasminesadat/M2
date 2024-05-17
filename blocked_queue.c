#include <stdio.h>
#include "blocked_queue.h"

void initBlockedQueue(blocked_queue *q)
{
    q->count = 0;
    for (int i = 0; i < QUEUE_SIZE; i++)
    {
        q->blockedQueue[i] = 0;
    }
}

void printBlockedQueue(blocked_queue *q)
{
    printf("Blocked Queue: \n");
    printf("head->[ ");
    for (int i = 0; i < QUEUE_SIZE; i++)
    {
        printf("%i%s", q->blockedQueue[i], i == QUEUE_SIZE - 1 ? "" : ", ");
    }
    printf(" ]\n");
}

void enqueueProcess(blocked_queue *q, int processID)
{
    q->blockedQueue[q->count] = processID;
    q->count = (q->count + 1) % QUEUE_SIZE;
}
void dequeueProcess(blocked_queue *q, int processID)
{
    int j;
    for (int i = 0; i < QUEUE_SIZE; i++)
    {
        if (processID == q->blockedQueue[i])
            j = i;
    }
    for (; j < QUEUE_SIZE - 1; j++)
    {
        q->blockedQueue[j] = q->blockedQueue[j + 1];
    }
    q->count--;
}