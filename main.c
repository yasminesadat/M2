#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "semaphore.h"
#include "blocked_queue.h"
#include "MLFQ.h"

// General Blocked Queue
blocked_queue blockedQueue;

// 3 Semaphores
semaphore file;
semaphore input;
semaphore output;
MLFQ scheduler;

typedef struct
{
    char programName[100];
    int arrivalTime;
} initalData;

// Get Data from User
initalData data[3];

typedef struct
{
    char name[100];
    char value[100];
} memoryElement;

// Global Array
memoryElement memory[60];

// Global Variables
int nextProcessID = 1; // assign to processes based on their arrival time
int i = 0;             // fill the memory

int clock = 0;
int arrived = 0; // count number of processes that have arrived

void printMemory();
void runOSsimulator();

//////////////////////////// HELPERS FOR MEMORY ///////////////////////////////////
char *getValue(char *processID, char *name)
{
    bool inProcessBlock = false;

    for (int i = 0; i < 60; i++)
    {
        if (strcmp("processID", memory[i].name) == 0)
        {
            if (strcmp(processID, memory[i].value) == 0)
            {
                inProcessBlock = true;
            }
            else
            {
                inProcessBlock = false;
            }
        }
        if (inProcessBlock && strcmp(memory[i].name, name) == 0)
        {
            return memory[i].value;
        }
    }
    printf("Variable does not exist for this process");
    return NULL;
}

void setValue(char *processID, char *name, char *value)
{
    bool inProcessBlock = false;

    for (int i = 0; i < 60; i++)
    {
        if (strcmp("processID", memory[i].name) == 0)
        {
            if (strcmp(processID, memory[i].value) == 0)
            {
                inProcessBlock = true;
            }
            else
            {
                inProcessBlock = false;
            }
        }
        if (inProcessBlock && strcmp(memory[i].name, name) == 0)
        {
            strcpy(memory[i].value, value);
        }
    }
}

void allocateMemory(char *programName, int arrivalTime)
{
    // Load Program
    int j = i + 11;
    FILE *filePointer;
    char line[100];

    // Open the file in read mode
    filePointer = fopen(programName, "r");

    // Check if file opened successfully
    if (filePointer == NULL)
    {
        perror("File could not be opened.");
    }

    char str[100];
    char numStr[2];
    // Read lines until end of file
    while (fgets(line, sizeof(line), filePointer) != NULL)
    {
        // Remove new line
        line[strcspn(line, "\n")] = '\0';
        // Store line in  memory
        sprintf(numStr, "%d", j - i - 11 + 1);
        strcpy(memory[j].name, "instruction ");
        strcat(memory[j].name, numStr);
        strcpy(memory[j++].value, line);
    }

    // Close the file
    fclose(filePointer);

    int rem = j - i - 11;
    // has now upperBound value
    j--;

    // add Process to MLFQ
    addProcess(&scheduler, nextProcessID);

    // 0
    strcpy(memory[i].name, "processID");
    sprintf(str, "%d", nextProcessID++);
    strcpy(memory[i++].value, str);
    // 1
    strcpy(memory[i].name, "state");
    strcpy(memory[i++].value, "READY");
    // 2
    strcpy(memory[i].name, "currPriority");
    sprintf(str, "%d", 1);
    strcpy(memory[i++].value, str);
    // 3
    strcpy(memory[i].name, "PC");
    sprintf(str, "%d", i + 8);
    strcpy(memory[i++].value, str);
    // 4
    strcpy(memory[i].name, "lowerBound");
    sprintf(str, "%d", i - 4);
    strcpy(memory[i++].value, str);
    // 5
    strcpy(memory[i].name, "upperBound");
    sprintf(str, "%d", j);
    strcpy(memory[i++].value, str);
    // 6
    strcpy(memory[i].name, "arrivalTime");
    sprintf(str, "%d", arrivalTime);
    strcpy(memory[i++].value, str);
    // 7
    strcpy(memory[i].name, "remainingTime");
    sprintf(str, "%d", rem);
    strcpy(memory[i++].value, str);
    // jump after the loaded instructions
    i = j + 1;
    printf("%s has been loaded into memory\n", programName);
    // return this print
    // printMemory();
}

void printMemory()
{
    for (int i = 0; i < 60; i++)
    {
        if (memory[i].name[0] != '\0')
            printf("%s%i | %s: %s\n", (i < 10 ? " " : ""), i, memory[i].name, memory[i].value);
        else
            printf("%s%i | \n", (i < 10 ? " " : ""), i);
    }
}
//////////////////////////// Interpreter functionality ////////////////////////////
void executeFromReady(int processID)
{
    // printf("Quantum is %i for Process %s\n", );
    //  char *token = strtok(instruction, " ");
    //  if (strcmp(token, "print") == 0)
    //  {
    //      token = strtok(NULL, " ");
    //      int index;
    //      for (index = 0; index < variable_count; index++)
    //      {
    //          if (strcmp(token, variables[index].name) == 0)
    //          {
    //              print_output(variables[index].value);
    //              break;
    //          }
    //      }
    //  }
    //  else if (strcmp(token, "assign") == 0)
    //  {
    //      token = strtok(NULL, " ");
    //      // char var_name[MAX_LENGTH];
    //      strcpy(var_name, token);
    //      token = strtok(NULL, " ");
    //      // char value[MAX_LENGTH];
    //      strcpy(value, token);
    //      assign_var(var_name, value);
    //  }
    //  else if (strcmp(token, "writeFile") == 0)
    //  {
    //      token = strtok(NULL, " ");
    //      // char filename[MAX_LENGTH];
    //      strcpy(filename, token);
    //      token = strtok(NULL, "\n");
    //      // char data[MAX_LENGTH];
    //      strcpy(data, token);
    //      write_to_file(filename, data);
    //  }
    //  else if (strcmp(token, "readFile") == 0)
    //  {
    //      token = strtok(NULL, "\n");
    //      // char filename[MAX_LENGTH];
    //      strcpy(filename, token);
    //      read_from_file(filename);
    //  }
    //  else if (strcmp(token, "printFromTo") == 0)
    //  {
    //      token = strtok(NULL, " ");
    //      int start = atoi(token);
    //      token = strtok(NULL, "\n");
    //      int end = atoi(token);
    //      print_from_to(start, end);
    //  }
    //  else if (strcmp(token, "semWait") == 0)
    //  {
    //  }
    //  else if (strcmp(token, "semSignal") == 0)
    //  {
    //  }
}

void print(char *value)
{
    printf("%s\n", value);
}

void printFromTo(int start, int end)
{
    for (int i = start; i <= end; i++)
    {
        printf("%d\n", i);
    }
}

void assign(char *processID, char *var_name, char *value)
{
    int index = 0;
    for (int i = 0; i < 60; i++)
    {

        if (strcmp(memory[i].name, "processID") == 0 && strcmp(memory[i].value, processID) == 0)
        {
            i += 8;
            if (memory[i].name[0] == '\0')
            {
                strcpy(memory[i].name, var_name);
                index = i;
                break;
            }
            else
            {
                if (strcmp(memory[i].name, var_name) == 0)
                {
                    index = i;
                    break;
                }
                else
                {
                    int j;
                    for (j = i + 1; j < (i + 3); j++)
                    {
                        if (memory[j].name[0] == '\0')
                        {
                            strcpy(memory[j].name, var_name);
                            index = j;
                            break;
                        }
                        else if (strcmp(memory[j].name, var_name) == 0)
                        {
                            index = j;
                            break;
                        }
                    }
                    if (index == 0)
                    {
                        printf("The Allocated Memory for process %s is full \n", processID);
                    }
                    break;
                }
            }
        }
    }
    if (index != 0)
    {
        if (strcmp(value, "input") == 0)
        {
            char input[100];
            char *endptr;
            printf("Please enter a value for %s: ", var_name);
            scanf("%s", input);
            strcpy(memory[index].value, input);
        }
        else
        {
            strcpy(memory[index].value, value);
        }
    }
}

void writeFile(char *processID, char *fileNameVar, char *dataVar)
{
    FILE *filePtr;

    char *fileName = getValue(processID, fileNameVar);
    char *dataValue = getValue(processID, dataVar);

    printf("Filename to be created: %s\n", fileName);
    printf("Value to be saved: %s\n", dataValue);
    // Open the file in write mode
    filePtr = fopen(fileName, "w");

    if (filePtr == NULL)
    {
        perror("Error: Unable to create file.\n");
    }
    fprintf(filePtr, "%s\n", dataValue);

    // Close the file
    fclose(filePtr);
}

void readFile(char *processID, char *fileNameVar)
{
    char *fileName = getValue(processID, fileNameVar);

    printf("Filename to be read: %s\n", fileName);

    FILE *file = fopen(fileName, "r");
    if (file != NULL)
    {
        char buffer[100];
        while (fgets(buffer, 100, file) != NULL)
        {
            printf("Values read from %s: %s\n", fileName, buffer);
        }
        fclose(file);
    }
    else
    {
        perror("Unable to open file for reading\n");
    }
}

bool semaphoreWait(int processID, char *semaphore)
{
    bool acquired;
    if (strcmp("userInput", semaphore) == 0)
    {
        acquired = semWait(&input, processID);
    }
    else if (strcmp("userOutput", semaphore) == 0)
    {
        acquired = semWait(&output, processID);
    }
    else if (strcmp("file", semaphore) == 0)
    {
        acquired = semWait(&file, processID);
    }
    if (!acquired)
    {
        enqueueProcess(&blockedQueue, processID);
        char *pid;
        sprintf(pid, "%d", processID);
        setValue(pid, "status", "BLOCKED");
    }
    return acquired;
}

void semaphoreSignal(int processID, char *semaphore)
{
    int nextProcess; // to acquire the resource
    if (strcmp("userInput", semaphore) == 0)
    {
        nextProcess = semSignal(&input, processID);
    }
    else if (strcmp("userOutput", semaphore) == 0)
    {
        nextProcess = semSignal(&output, processID);
    }
    else if (strcmp("file", semaphore) == 0)
    {
        nextProcess = semSignal(&file, processID);
    }
    if (nextProcess != -1)
    { // add to MLFQ again the blocked process if exists
        char *pid;
        sprintf(pid, "%d", processID);
        char *currPriority = getValue(pid, "currPriority");
        returnProcess(&scheduler, processID, atoi(currPriority));
    }
}
/////////////////////////////  SIMULATOR  ///////////////////////////////
void OSsetUp()
{
    // uncomment later
    //  char *name;
    // int time;
    // for (int i = 0; i < 3; i++)
    // {
    //     printf("Enter name of program to run:\n");
    //     scanf("%s", name);
    //     printf("Enter program arrival time:\n");
    //     scanf("%d", &time);
    //     strcpy(data[i].programName, name);
    //     data[i].arrivalTime = time;
    // }
    for (int i = 0; i < 3; i++)
    {
        printf("%s will arrive at cycle = %d\n", data[i].programName, data[i].arrivalTime);
    }

    initSemaphore(&input);
    initSemaphore(&input);
    initSemaphore(&output);
    initBlockedQueue(&blockedQueue);
    initMLFQ(&scheduler);
    runOSsimulator();
}

char *processArrived()
{
    for (int i = 0; i < 3; i++)
    {
        if (data[i].arrivalTime == clock)
        {
            data[i].arrivalTime = -1;
            return data[i].programName;
        }
    }
    return NULL;
}

void runOSsimulator()
{
    while (1)
    {
        printf("\n--------- CLOCK CYCLE %i ---------\n", clock);
        if (arrived < 3)
        {
            char *program = processArrived();
            while (program != NULL)
            {
                allocateMemory(program, clock);
                arrived++;
                program = processArrived();
            }
        }
        int processID = getProcess(&scheduler);
        // check end of simulator
        if (processID == -1)
        {
            return;
        }

        char pid[100];
        sprintf(pid, "%d", processID);
        char *PCValue = getValue(pid, "PC");
        char *endptr;
        int PC = strtol(PCValue, &endptr, 10);
        int quantum = scheduler.currQuantum;
        printf("Process %i now has a quantum of %i\n", processID, quantum);
        if (clock == 9)
            return;
        clock++;
    }
}

int main()
{

    strcpy(data[0].programName, "Program_1.txt");
    data[0].arrivalTime = 0;

    strcpy(data[1].programName, "Program_2.txt");
    data[1].arrivalTime = 0;

    strcpy(data[2].programName, "Program_3.txt");
    data[2].arrivalTime = 2;
    OSsetUp();
    printMLFQ(&scheduler);
    // queue q;
    // initQueue(&q);
    // enqueue(&q, 1);
    // enqueue(&q, 3);
    // dequeue(&q);
    // dequeue(&q);
    // dequeue(&q);
    return 0;
}

////////////////////////////// HELPER INFO /////////////////////////////////////////

// Total : 12 lines
// PCB: processID
//      state->NEW, READY, RUNNING, BLOCKED, FINISHED
//      currPriority
//      PC
//      lowerBound
//      upperBound

// Process Info: arrivalTime
//               remainingTime
//               variableNext
// then variable 1,2,3