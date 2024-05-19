#include <stdio.h>
#include <stdbool.h>
#include "priority_queue.h"

void initPQueue(priority_queue *q)
{
    q->count = 0;
    queue_element elem = {0, 0};
    for (int i = 0; i < QUEUE_SIZE; i++)
    {
        q->elements[i] = elem;
    }
}

void printPQueue(priority_queue *q)
{
    printf("head->[ ");
    for (int i = 0; i < QUEUE_SIZE; i++)
    {
        printf("( %i,%i )%s", q->elements[i].pid, q->elements[i].priority, i == QUEUE_SIZE - 1 ? "" : ", ");
    }
    printf(" ]\n");
}

bool isEmptyP(priority_queue *q)
{
    return (q->count == 0);
}

bool isFullP(priority_queue *q)
{
    return (q->count == QUEUE_SIZE);
}

void enqueueP(priority_queue *q, int processID, int priority)
{
    if (!isFullP(q))
    {
        int i;
        queue_element elem = {processID, priority};
        for (i = q->count - 1; i >= 0; i--)
        {
            if (q->elements[i].priority > priority)
            {
                q->elements[i + 1] = q->elements[i];
            }
            else
            {
                break;
            }
        }

        // Insert the new element
        q->elements[i + 1] = elem;
        q->count++;
    }
    else
    {
        printf("Queue is Full\n");
    }
}

int dequeueP(priority_queue *q)
{
    if (!isEmptyP(q))
    {
        int processID = q->elements[0].pid;
        for (int i = 0; i < QUEUE_SIZE - 1; i++)
        { // shifting
            q->elements[i] = q->elements[i + 1];
        }
        q->count--;
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