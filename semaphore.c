#include <stdbool.h>
#include <stdio.h>
#include "semaphore.h"

void semWait(semaphore *s, int processID)
{
    if (s->ava)
    {
        s->ava = false;
        s->ownerID = processID;
        printf("Resource is now acquired by process %i\n", processID);
    }
    else if (s->ownerID == processID)
    {
        printf("Resource is already acquired by the process");
    }
    else
    {
        printf("Process %i is now blocked for the resource\n", processID);
        enqueue(&(s->blockedQueue), processID);
    }
}

void semSignal(semaphore *s, int processID)
{
    if (processID == s->ownerID)
    {
        if (!isEmpty(&(s->blockedQueue)))
        {
            int dequeuedProcessID = dequeue(&(s->blockedQueue));
            printf("Resource is now acquired by process %i\n", dequeuedProcessID);
            s->ownerID = dequeuedProcessID;
        }
        else
        {
            s->ava = true;
            printf("Resource is now available\n");
        }
    }
}
