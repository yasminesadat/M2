#include <stdio.h>
#include <stdbool.h>
#include "queue.h"

void initQueue(queue *q)
{
    q->count = 0;
    for (int i = 0; i < QUEUE_SIZE; i++)
    {
        q->Elements[i] = 0;
    }
}

void printQueue(queue *q)
{
    printf("head->[ ");
    for (int i = 0; i < QUEUE_SIZE; i++)
    {
        printf("%i%s", q->Elements[i], i == QUEUE_SIZE - 1 ? "" : ", ");
    }
    printf(" ]\n");
}

bool isEmpty(queue *q)
{
    return (q->count == 0);
}

bool isFull(queue *q)
{
    return (q->count == QUEUE_SIZE);
}

void enqueue(queue *q, int processID)
{
    if (!isFull(q))
    {
        q->Elements[q->count++] = processID;
        printf("After enqueue: \n");
        printQueue(q);
    }
    else
    {
        printf("Queue is Full\n");
    }
}

int dequeue(queue *q)
{
    if (!isEmpty(q))
    {
        int processID = q->Elements[0];
        for (int i = 0; i < QUEUE_SIZE - 1; i++)
        { // shifting
            q->Elements[i] = q->Elements[i + 1];
        }
        q->count--;
        printf("After dequeue: \n");
        printQueue(q);
        return processID;
    }
    else
    {
        printf("Queue is Empty\n");
        return -1;
    }
}
