#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "queue.h"

typedef struct
{
    bool ava;
    queue blockedQueue;
    int ownerID;
} semaphore;

void semWait(semaphore *s, int processID);
void semSignal(semaphore *s, int processID);

#endif