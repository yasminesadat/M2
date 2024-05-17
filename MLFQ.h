#ifndef MLFQ_H
#define MLFQ_H

#include "queue.h"

typedef struct
{
    queue queues[4];
    int timeQuanta[4];
    int currQuantum;
} MLFQ;

void initMLFQ(MLFQ *mlfq);
void printMLFQ(MLFQ *mlfq);
void addProcess(MLFQ *mlfq, int processID);
void returnProcess(MLFQ *mlfq, int processID, int currentLevel);
int getProcess(MLFQ *mlfq);
int demoteProcess(MLFQ *mlfq, int processID, int currentLevel);

#endif