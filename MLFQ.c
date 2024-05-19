#include <stdio.h>
#include "MLFQ.h"

void initMLFQ(MLFQ *mlfq)
{
    for (int i = 0; i < 4; i++)
    {
        initQueue(&mlfq->queues[i]);
    }
    mlfq->timeQuanta[0] = 1;
    mlfq->timeQuanta[1] = 2;
    mlfq->timeQuanta[2] = 4;
    mlfq->timeQuanta[3] = 8;
    mlfq->currQuantum = 1;
}

void printMLFQ(MLFQ *mlfq)
{
    for (int i = 0; i < 4; i++)
    {
        printf("Level %i\n", i + 1);
        printQueue(&(mlfq->queues[i]));
    }
}

void addProcess(MLFQ *mlfq, int processID)
{
    enqueue(&mlfq->queues[0], processID);
}

void placeProcess(MLFQ *mlfq, int processID, int currentLevel)
{
    enqueue(&mlfq->queues[currentLevel - 1], processID);
}

int getProcess(MLFQ *mlfq)
{
    for (int i = 0; i < 4; i++)
    {
        if (!isEmpty(&(mlfq->queues[i])))
        {
            mlfq->currQuantum = mlfq->timeQuanta[i];
            return dequeue(&mlfq->queues[i]);
        }
    }
    return -1; // No process found
}
