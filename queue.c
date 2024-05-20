#include <stdio.h>
#include <stdbool.h>
#include "queue.h"

void initQueue(queue *q)
{
    q->count = 0;
    for (int i = 0; i < QUEUE_SIZE; i++)
    {
        q->elements[i] = 0;
    }
}

void printQueue(queue *q)
{
    printf("head->[ ");
    for (int i = 0; i < QUEUE_SIZE; i++)
    {
        printf("%i%s", q->elements[i], i == QUEUE_SIZE - 1 ? "" : ", ");
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
        q->elements[q->count++] = processID;
        // printf("After enqueue: \n");
        // printQueue(q);
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
        int processID = q->elements[0];
        for (int i = 0; i < QUEUE_SIZE - 1; i++)
        { // shifting
            q->elements[i] = q->elements[i + 1];
        }
        q->count--;
        q->elements[q->count] = 0;
        // printf("After dequeue: \n");
        // printQueue(q);
        return processID;
    }
    else
    {
        printf("Queue is Empty\n");
        return -1;
    }
}
