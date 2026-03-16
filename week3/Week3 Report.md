# Overview of Week 3 activities
Each activity focused on parallel computing using MPI(Message Passing Interface). 
This allows for the splitting of one job into multiple smaller jobs so multiple processors can work on them at the same time.
Main aims were to compile and run MPI code, benchmark the difference between real time and CPU time, and finally implemented a parallel vector addition system.

## Instructions
### Activity 1:
For the first activity, a program called *hello_mpi.c* was created using the example in the class notes to test how multiple processors communicate with one another.
Following the basic C template, it was modified to inlcude *mpi.h* header
```
include <mpi.h>
```
Code was written to include *MPI_Init* to start the parallel enviroment, *MPI_Comm_Rank* to give each processor an ID number, and *MPI_Comm_size* to count how many processors were being used.
```
int myRank, uniSize, ierror;
    ierror = MPI_Init(&argc, &argv); 
    ierror = MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    ierror = MPI_Comm_size(MPI_COMM_WORLD, &uniSize);
```
A printf statement was added to show the rank and total size of each parallel group. 
FInally *MPI_Finalize* was added to close all the connections.

The code was compiled using *mpicc* instead of gcc compiler as mpicc acts as a compiler wrapper. It calls gcc but also calls the MPI libraries and headers needed for parallel computing.
```
mpicc HPQC/week3/hello_mpi.c -o bin/hello_mpi
```
Once this was done, the program could be ran on several processors at once. By using the *mpirun* command with *-np 4* flag, the work was split between 4 seperate processes.
It was timed externally using the *time* command to see how long it takes various amount of processors to carry out the task.
```
time mpirun -np 4 bin/hello_mpi
```
Tests were conducted by omitting MPI_Init, which caused the program to crash, and omitting MPI_Finalize, which prevented the program from shutting down correctly.

### Acivity 2:
The second activity centred around analysisng *proof.c* to understand how a MPI splits the work between a root node and  multiple client modes.
The code was examined to see how it uses point to point communication, *MPI_Send* and *MPI_Recv*, to aggregate data.

Whilst reading through the code, a short summary of each functions purpose was recorded, and diagrams were created to show the codes structure. 
It was seen that the script could be simplifed using a mathematical formula to acheive the same result as the parallel loop, which wouldd optimise the script.

### Activity 3:
The final activty involved comparing serial vector addition to a parallel version.
The serial program *vector_serial.c* was examined and documented as similarly carried out in activity 2.
It was then modified to create a baseline for adding numbers. 
First, to stop the program from crashing from large inputs, malloc was used to set aside enough space for the list based on the number provided by the user.
```
int* my_vector = malloc (num_arg * sizeof(int));
initialise_vector(my_vector, num_arg, 0);
```
It was then compiled and timed to established a performance baseline.
```
gcc PHY1090/week3/vector_serial.c -o bin/vector_serial
time ./bin/vector_serial 10
```
The code was then modified to perform a non-trivial mathematical operation and benchmarked internally and externally.
To make it non-trivial, a loop was added that fills every spot with the square of its index, instead of leaving them as zeros.
```
for (int i = 0; i < size; i++)
{vector[i] = initial;}
```
An internal benchmarking was added using *time.h* library. The *clock* function was used to take a snapshot of the time right before the math startes and end, which allowed the program to report exactly howlong the calculation took, not including how long it took for the whole program to run.
```
clock_t start = clock();
# non trivial operation
// end benchmark
clock_t end = clock();
double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
```
//The total time was printed, and
The code was then compiled with gcc and timed externally to see how long it takes to run the program.
```
gcc week3/vector_serial.c -o bin/vector_serial
time ./bin/vector_serial 1000000
````

Following this, a parallel version was created called *vector_mpi.c*, to see if splitting the amount of work across multiple processors improves on the speed. 
The program uses MPI_Comm_rank and MPI_Comm_size to identify each processor and determine the total number of workers available.
To divide the work equally, a chunk system was implemented, where the total number of elements was divided by the number of processors to find a start and end point for each rank.
This ensures that the procesors only calculated its assigned portion of the math. 
A check is applied at the last rank to check if there is any leftover elements, they are included in the toal if they didnt divide perfectly.
```
int chunk = total_elements / size;
int start = rank * chunk;
int stop = (rank + 1) * chunk;
if (rank == size -1) stop = total_elements;
```
Inside the loop, each processor calculated the sum of squares for its chunk. Once finsihed, the workers(non zero ranks) used MPI_Send to transmit their results to the root.
```
if (rank != 0)
{MPI_Send(&partial_sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);}
```
The root starts with its own chunk, then waits for the other processors to finish their work. Using a forr loop, the root uses *MPI_Recv* to get the answer from each worker one by one to add it to the finl total.
```
int final_sum = partial_sum;
for (int source = 1; source < size; source++){
MPI_Recv(&received_sum, 1, MPI_INT, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
 final_sum += received_sum;}
```
This version was compiled with mpicc and tested using mpirun to compare its performance against the serial version.
```
mpicc week3/vector_mpi.c -o bin/vector_mpi
time mpirun -np 4 bin/vector_mpi 1000000
```

## Results

### Activity 1:
### Activity 2:
### Activity 3:

## Discusion

### Activity 1:
### Activity 2:
### Activity 3:
