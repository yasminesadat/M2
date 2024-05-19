#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

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
    int remainingTime;
} programData;

// Get Data from User
programData data[3];

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
void trimWhitespace(char *str)
{
    int length = strlen(str);
    while (length > 0 && isspace((unsigned char)str[length - 1]))
    {
        str[length - 1] = '\0';
        length--;
    }
}

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
    int j = i + 9;
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
        sprintf(numStr, "%d", j - i - 8);
        strcpy(memory[j].name, "instruction ");
        strcat(memory[j].name, numStr);
        strcpy(memory[j++].value, line);
    }

    // Close the file
    fclose(filePointer);

    int rem = j - i - 9;
    // has now upperBound value
    j--;

    // add Process to MLFQ
    addProcess(&scheduler, nextProcessID);

    // 0
    strcpy(memory[i].name, "processID");
    sprintf(str, "%d", nextProcessID);
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
    sprintf(str, "%d", i + 6);
    strcpy(memory[i++].value, str);
    // 4
    strcpy(memory[i].name, "lowerBound");
    sprintf(str, "%d", i - 4);
    strcpy(memory[i++].value, str);
    // 5
    strcpy(memory[i].name, "upperBound");
    sprintf(str, "%d", j);
    strcpy(memory[i++].value, str);

    // in process info
    data[nextProcessID - 1].remainingTime = rem;
    // jump after the loaded instructions
    i = j + 1;
    nextProcessID++;
    printf("%s has been loaded into memory\n", programName);
}

void printMemory()
{
    printf("\n");
    for (int i = 0; i < 60; i++)
    {
        if (memory[i].name[0] != '\0')
            printf("%s%i | %s: %s\n", (i < 10 ? " " : ""), i, memory[i].name, memory[i].value);
        else
            printf("%s%i | \n", (i < 10 ? " " : ""), i);
    }
}
//////////////////////////// Interpreter functionality ////////////////////////////

void print(char *processID, char *variable)
{
    char *value = getValue(processID, variable);
    if (value == NULL)
    {
        printf("Variable doesn't exist for this process\n");
        return;
    }
    printf("%s\n", value);
}

void printFromTo(char *processID, char *startVar, char *endVar)
{
    char *first = getValue(processID, startVar);
    char *last = getValue(processID, endVar);
    if (first == NULL || last == NULL)
    {
        printf("Variable doesn't exist for this process\n");
        return;
    }
    int start = atoi(first);
    int end = atoi(last);
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
            i += 6;
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
    if (dataValue == NULL || fileName == NULL)
    {
        printf("Variable doesn't exist for this process\n");
        return;
    }

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
    if (fileName == NULL)
    {
        printf("Variable doesn't exist for this process\n");
        return;
    }

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
    char pid[100];
    sprintf(pid, "%d", processID);
    if (strcmp(semaphore, "userInput") == 0)
    {
        printf("UserInput Semaphore: ");
        acquired = semWait(&input, processID, atoi(getValue(pid, "currPriority")));
    }
    else if (strcmp(semaphore, "userOutput") == 0)
    {
        printf("UserOutput Semaphore: ");
        acquired = semWait(&output, processID, atoi(getValue(pid, "currPriority")));
    }
    else if (strcmp(semaphore, "file") == 0)
    {
        printf("File Semaphore: ");
        acquired = semWait(&file, processID, atoi(getValue(pid, "currPriority")));
    }
    if (!acquired)
    {
        // add to blocked queue
        enqueueProcess(&blockedQueue, processID);
        // update status
        setValue(pid, "state", "BLOCKED");
    }
    return acquired;
}

void semaphoreSignal(int processID, char *semaphore)
{
    int nextProcess; // to acquire the resource
    trimWhitespace(semaphore);
    if (strcmp("userInput", semaphore) == 0)
    {
        printf("UserInput Semaphore: ");
        nextProcess = semSignal(&input, processID);
    }
    else if (strcmp("userOutput", semaphore) == 0)
    {
        printf("UserOutput Semaphore: ");
        nextProcess = semSignal(&output, processID);
    }
    else if (strcmp("file", semaphore) == 0)
    {
        printf("File Semaphore: ");
        nextProcess = semSignal(&file, processID);
    }
    if (nextProcess != -1)
    {
        char *pid;
        sprintf(pid, "%d", nextProcess);
        char *currPriority = getValue(pid, "currPriority");
        // remove from blocked queue
        dequeueProcess(&blockedQueue, nextProcess);
        //  add to MLFQ again the blocked process if exists
        placeProcess(&scheduler, nextProcess, atoi(currPriority));
        // update its state
        setValue(pid, "state", "READY");
    }
}

char *readLine(char *processID, char *fileNameVar)
{
    char *fileName = getValue(processID, fileNameVar);

    printf("Filename to be read: %s\n", fileName);

    FILE *file = fopen(fileName, "r");
    if (file != NULL)
    {
        char tempBuffer[100];
        if (fgets(tempBuffer, 100, file) != NULL)
        {
            fclose(file);

            // Allocate memory for the return buffer
            char *buffer = malloc(sizeof(tempBuffer));
            if (buffer == NULL)
            {
                perror("Unable to allocate memory\n");
                return NULL;
            }
            trimWhitespace(tempBuffer);

            // Copy the string into the allocated buffer
            strcpy(buffer, tempBuffer);

            printf("Values read from %s: %s\n", fileName, buffer);
            return buffer;
        }
        else
        {
            fclose(file);
        }
    }
    else
    {
        perror("Unable to open file for reading");
    }
    return NULL;
}

bool executeInstruction(int processID, int PC)
{
    char pid[100];
    sprintf(pid, "%d", processID);
    // avoid manipulation of memory with null terminators
    char tempStr[100];
    strcpy(tempStr, memory[PC].value);
    printf("Instruction to be executed: %s\n", tempStr);

    char *token = strtok(tempStr, " ");
    if (strcmp(token, "print") == 0)
    {
        token = strtok(NULL, " ");
        trimWhitespace(token);
        print(pid, token);
    }
    else if (strcmp(token, "printFromTo") == 0)
    {
        token = strtok(NULL, " ");
        trimWhitespace(token);
        char *start = token;
        token = strtok(NULL, " ");
        trimWhitespace(token);
        char *end = token;
        printFromTo(pid, start, end);
    }
    else if (strcmp(token, "assign") == 0)
    {
        token = strtok(NULL, " ");
        trimWhitespace(token);
        char *variable = token;
        token = strtok(NULL, " ");
        trimWhitespace(token);
        char *value = token;
        if (strcmp("readFile", value) == 0)
        {
            token = strtok(NULL, " ");
            trimWhitespace(token);
            char *line = readLine(pid, token);
            assign(pid, variable, line);
            free(line);
        }
        else
        {
            assign(pid, variable, value);
        }
    }
    else if (strcmp(token, "writeFile") == 0)
    {
        token = strtok(NULL, " ");
        trimWhitespace(token);
        char *filename = token;
        token = strtok(NULL, " ");
        trimWhitespace(token);
        char *data = token;
        writeFile(pid, filename, data);
    }
    else if (strcmp(token, "readFile") == 0)
    {
        token = strtok(NULL, " ");
        trimWhitespace(token);
        readFile(pid, token);
    }
    else if (strcmp(token, "semWait") == 0)
    {
        token = strtok(NULL, " ");
        trimWhitespace(token);
        return semaphoreWait(processID, token);
    }
    else if (strcmp(token, "semSignal") == 0)
    {
        token = strtok(NULL, " ");
        trimWhitespace(token);
        semaphoreSignal(processID, token);
    }
    return true;
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
        printf("%s will arrive at t = %d\n", data[i].programName, data[i].arrivalTime);
    }

    initSemaphore(&input);
    initSemaphore(&file);
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

void printSemaphores()
{
    printf("UserInput Semaphore PQ: ");
    printPQueue(&(input.blockedQueue));
    printf("UserOutput Semaphore PQ: ");
    printPQueue(&(output.blockedQueue));
    printf("File Semaphore PQ: ");
    printPQueue(&(file.blockedQueue));
}

void runOSsimulator()
{
    int processID;
    do
    {
        printf("\n--------- CLOCK CYCLE AT t = %i ---------\n", clock);
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
        printBlockedQueue(&blockedQueue);
        printMLFQ(&scheduler);
        processID = getProcess(&scheduler);
        // check end of simulator
        if (processID == -1)
        {
            return;
        }
        char pid[100];
        sprintf(pid, "%d", processID);
        char *PCValue = getValue(pid, "PC");
        int PC = atoi(PCValue);
        int quantum = scheduler.currQuantum;
        int remaining = data[processID - 1].remainingTime;
        int running = remaining < quantum ? remaining : quantum;
        printf("Process %i now has a quantum of %i and wants to run for %i clock cycle(s)\n", processID, quantum, running);
        setValue(pid, "state", "RUNNING");
        printMemory();
        printSemaphores();
        for (int i = 0; i < running; i++)
        {
            // last instruction in quantum so demote process priority
            if (i == running - 1)
            {
                int currPriority = atoi(getValue(pid, "currPriority"));
                int newPriority = currPriority == 4 ? currPriority : currPriority + 1;
                char newValue[100];
                sprintf(newValue, "%d", newPriority);
                setValue(pid, "currPriority", newValue);
            }
            bool success = executeInstruction(processID, PC);

            // increment PC and decrement Remaining Time
            PC += 1;
            char newPC[100];
            sprintf(newPC, "%d", PC);
            setValue(pid, "PC", newPC);
            data[processID - 1].remainingTime -= 1;
            if (!success)
            {
                break;
            }

            if (success && i < running - 1)
            {
                clock++;
                printf("\n--------- CLOCK CYCLE AT t = %i ---------\n", clock);
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
                printBlockedQueue(&blockedQueue);
                printMLFQ(&scheduler);
                printMemory();
                printSemaphores();
            }
        }
        // done with process
        if (data[processID - 1].remainingTime == 0)
        {
            setValue(pid, "state", "FINISHED");
        }
        // process is demoted if not blocked
        else if (strcmp(getValue(pid, "state"), "BLOCKED") != 0)
        {
            placeProcess(&scheduler, processID, atoi(getValue(pid, "currPriority")));
            setValue(pid, "state", "READY");
        }

        clock++;
    } while (1);
}

int main()
{
    strcpy(data[0].programName, "Program_1.txt");
    data[0].arrivalTime = 0;

    strcpy(data[1].programName, "Program_2.txt");
    data[1].arrivalTime = 1;

    strcpy(data[2].programName, "Program_3.txt");
    data[2].arrivalTime = 3;
    OSsetUp();
    printSemaphores();
    return 0;
}

////////////////////////////// HELPER INFO /////////////////////////////////////////

// Total : 9 lines
// PCB: processID
//      state-> READY, RUNNING, BLOCKED, FINISHED
//      currPriority
//      PC
//      lowerBound
//      upperBound

// then variable 1,2,3