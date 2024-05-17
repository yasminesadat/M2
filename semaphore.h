#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "queue.h"

typedef struct
{
    bool ava;
    queue blockedQueue;
    int ownerID;
} semaphore;

void initSemaphore(semaphore *s);
bool semWait(semaphore *s, int processID);
int semSignal(semaphore *s, int processID); // returns next processID to use the resource

#endif