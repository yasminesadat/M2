#include <stdbool.h>
#include <stdio.h>
#include "semaphore.h"

void initSemaphore(semaphore *s)
{
    initPQueue(&(s->blockedQueue));
    s->ava = true;
}

bool semWait(semaphore *s, int processID, int priority)
{
    if (s->ava)
    {
        s->ava = false;
        s->ownerID = processID;
        printf("Resource is now acquired by process %i\n", processID);
        return true;
    }
    else if (s->ownerID == processID)
    {
        printf("Resource is already acquired by the process\n");
        return true;
    }
    else
    {
        printf("Process %i is now blocked for the resource being used by Process %i\n", processID, s->ownerID);
        enqueueP(&(s->blockedQueue), processID, priority);
        return false;
    }
}

int semSignal(semaphore *s, int processID)
{
    if (processID == s->ownerID)
    {
        if (!isEmptyP(&(s->blockedQueue)))
        {
            int dequeuedProcessID = dequeueP(&(s->blockedQueue));
            printf("Resource can now be used by process %i\n", dequeuedProcessID);
            s->ownerID = dequeuedProcessID;
            return dequeuedProcessID;
        }
        else
        {
            s->ava = true;
            printf("Resource is now available\n");
            return -1;
        }
    }
    return -1;
}
