#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct
{
    char name[100];
    char value[100];
} MemoryElement;

typedef struct
{
    int processID;
    int arrivalTime;
    int remainingTime;
    int variableNext;
    int variableCount; // how many variables are there for this process with max of 3
} ProcessInfo;

// Global Array
MemoryElement memory[60];

// Global Variables
int nextProcessID = 1;
int i = 0; // fill the memory

//////////////////////////// HELPERS FOR MEMORY ///////////////////////////////////
char *getValue(char *processID, char *name)
{
    char inProcessBlock = 'f';

    for (int i = 0; i < 60; i++)
    {
        if (strcmp("processID", memory[i].name) == 0)
        {
            if (strcmp(processID, memory[i].value) == 0)
            {
                inProcessBlock = 't';
            }
            else
            {
                inProcessBlock = 'f';
            }
        }
        if (inProcessBlock == 't' && strcmp(memory[i].name, name) == 0)
        {
            return memory[i].value;
        }
    }
    perror("Variable does not exist for this process");
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

    // Read lines until end of file
    while (fgets(line, sizeof(line), filePointer) != NULL)
    {
        // Remove new line
        line[strcspn(line, "\n")] = '\0';
        // Store line in  memory
        strcpy(memory[j++].value, line);
    }

    // Close the file
    fclose(filePointer);

    int rem = j - i - 11;
    // has now upperBound value
    j--;

    char str[100];
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
}

void printMemory()
{
    for (int i = 0; i < 60; i++)
    {
        if (memory[i].name[0] != '\0')
            printf("%s%i | %s: %s\n", (i < 10 ? " " : ""), i, memory[i].name, memory[i].value);
        else
            printf("%s%i | %s\n", (i < 10 ? " " : ""), i, memory[i].value);
    }
}
//////////////////////////// Interpreter functionality ////////////////////////////
int power(int base, int exponent)
{
    int result = 1;
    for (int i = 0; i < exponent; ++i)
    {
        result *= base;
    }
    return result;
}
void executeFromReady(char *processID)
{
    char *priority = getValue(processID, "currPriority");
    char *endpt;
    int quantum = power(2, strtol(priority, &endpt, 10) - 1);
    printf("Quantum is %i for Process %s\n", quantum, processID);
    // char *token = strtok(instruction, " ");
    // if (strcmp(token, "print") == 0)
    // {
    //     token = strtok(NULL, " ");
    //     int index;
    //     for (index = 0; index < variable_count; index++)
    //     {
    //         if (strcmp(token, variables[index].name) == 0)
    //         {
    //             print_output(variables[index].value);
    //             break;
    //         }
    //     }
    // }
    // else if (strcmp(token, "assign") == 0)
    // {
    //     token = strtok(NULL, " ");
    //     // char var_name[MAX_LENGTH];
    //     strcpy(var_name, token);
    //     token = strtok(NULL, " ");
    //     // char value[MAX_LENGTH];
    //     strcpy(value, token);
    //     assign_var(var_name, value);
    // }
    // else if (strcmp(token, "writeFile") == 0)
    // {
    //     token = strtok(NULL, " ");
    //     // char filename[MAX_LENGTH];
    //     strcpy(filename, token);
    //     token = strtok(NULL, "\n");
    //     // char data[MAX_LENGTH];
    //     strcpy(data, token);
    //     write_to_file(filename, data);
    // }
    // else if (strcmp(token, "readFile") == 0)
    // {
    //     token = strtok(NULL, "\n");
    //     // char filename[MAX_LENGTH];
    //     strcpy(filename, token);
    //     read_from_file(filename);
    // }
    // else if (strcmp(token, "printFromTo") == 0)
    // {
    //     token = strtok(NULL, " ");
    //     int start = atoi(token);
    //     token = strtok(NULL, "\n");
    //     int end = atoi(token);
    //     print_from_to(start, end);
    // }
    // else if (strcmp(token, "semWait") == 0)
    // {
    // }
    // else if (strcmp(token, "semSignal") == 0)
    // {
    // }
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
            // printf("i value here %i: \n", i);
            char *n = memory[i].name;
            if (n[0] == '\0')
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
                        char *n2 = memory[j].name;
                        if (n2[0] == '\0')
                        {
                            strcpy(memory[j].name, var_name);
                            index = j;
                            break;
                        }
                        else if (strcmp(n2, var_name) == 0)
                        {
                            index = j;
                            break;
                        }
                    }
                    char *n3 = memory[++j].name;
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
/////////////////////////////////////////////////////////////////////////////////

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

int main()
{
    allocateMemory("Program_1.txt", 1);
    allocateMemory("Program_2.txt", 3);
    allocateMemory("Program_3.txt", 3);
    assign("2", "x", "22");
    assign("2", "x", "filee");
    assign("2", "y", "4");
    assign("2", "z", "4");
    assign("2", "y", "yy");
    // assign("1", "bla", "filee");
    printMemory();
    // writeFile("2", "x", "y");
    readFile("1", "x");
    return 0;
}