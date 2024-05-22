# OS-Simulator
This project is a simulation of an operating system using Multilevel Feedback Queue Scheduling and Mutexes. The scheduler has four queues with quantums 1,2,4,and 8 respectively. The mutexes available control resource sharing across 3 resources: file reading and writing, input from user, and output to the screen. Each mutex has a priority queue to gauarantee that the process that returns to the MLFQ is the one with the highest priority, where 1 is the highest priority and 3 is the lowest.

**Set Up:**

You need to have 3 .txt files representing the programs to be run in the same folder where the code is. The simulator first asks the user to enter the program names and their arrival times. Then, a PCB is created in memory for each process when the clock ticks at the time it should arrive. It is to be noted that the memory implemented has a finite space of 60 words, storing the instructions and PCBs. 

**Programs Interpreter:**

For the programs, the following syntax is used:

● print: to print the output on the screen. Example: print x.

● assign: to initialize a new variable and assign a value to it. Example: assign x y, where x is
the variable and y is the value assigned. The value could be an integer number, or a string. If
y is input, it first prints to the screen "Please enter a value", then the value is taken as an
input from the user.

● writeFile: to write data to a file. Example: writeFile x y, where x is the filename and y is the
data.

● readFile: to read data from a file. Example: readFile x, where x is the filename.

● printFromTo: to print all numbers between 2 numbers. Example: printFromTo x y, where x
is the first number, and y is the second number.

● semWait: to acquire a resource. Example: semWait x, where x is the resource name. For
more details refer to section Mutual Exclusion.

● semSignal: to release a resource. Example: semSignal x, where x is the resource name. For
more details refer to section Mutual Exclusion.
