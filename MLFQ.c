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
}

void addProcess(MLFQ *mlfq, int processID)
{
    enqueue(&mlfq->queues[0], processID);
}

void returnProcess(MLFQ *mlfq, int processID, int currentLevel)
{
    enqueue(&mlfq->queues[currentLevel - 1], processID);
}

int getProcess(MLFQ *mlfq)
{
    for (int i = 0; i < 4; i++)
    {
        if (!isEmpty(&mlfq->queues[i]))
        {
            return dequeue(&mlfq->queues[i]);
        }
    }
    return -1; // No process found
}

int demoteProcess(MLFQ *mlfq, int processID, int currentLevel)
{
    if (currentLevel < 4)
    {
        enqueue(&mlfq->queues[currentLevel], processID);
        return currentLevel + 1;
    }
    else
    {
        enqueue(&mlfq->queues[3], processID); // Already at lowest priority
        return currentLevel;
    }
}