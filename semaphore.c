#include <stdbool.h>
#include "semaphore.h"

void semWait(semaphore *s, int processID)
{
    if (s->ava)
    {
        s->ava = false;
    }
    else
    {
        enqueue(&(s->blockedQueue), processID);
    }
}

void semSignal(semaphore *s, int processID)
{
    if (processID == s->ownerID)
    {
        if (!isEmpty(&(s->blockedQueue)))
        {
            int processID = dequeue(&(s->blockedQueue));
            s->ownerID = processID;
        }
        else
        {
            s->ava = true;
        }
    }
}
