#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "priority_queue.h"

typedef struct
{
    bool ava;
    priority_queue blockedQueue;
    int ownerID;
} semaphore;

void initSemaphore(semaphore *s);
bool semWait(semaphore *s, int processID, int priority);
int semSignal(semaphore *s, int processID); // returns next processID to use the resource

#endif