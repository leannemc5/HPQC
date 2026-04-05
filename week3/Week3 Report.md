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
Each execution of the program printed a *No protocol specified*  warning from the MPI enviroment, followed by *Hello World* from each process in a non sequential order, showing the asynchronous nature of the parallel execution.

### Activity 2:
The overall idea of the proof.c code is to run the code using multiple processes. Each process gets a rank; Rank 0 is the root, while every other rank (1, n-1) is a client. Each client computes a values and sends it to the root, where it collects all the values and sums them together.


| Processes (-np) | Real Time (s) | User Time (s) | Sys Time (s) | 
| :--- | :--- | :--- | :--- | 
| 4 | 0.415 | 0.096 | 0.183 |
| 8 | 0.430 | 0.189 | 0.291 | 
| 12 | 0.453 | 0.246 | 0.414 |
| 16 | 0.474 | 0.402 | 0.458 |

In the serial execution, each *Hello World* statement is printed in sequential order from process 0 to n-1, confirming that tasks are handled one after another by a single core.

| Processes (-np) | Real Time (s) | User Time (s) | Sys Time (s) |
| :--- | :--- | :--- | :--- |
| 4 | 0.005 | 0.000 | 0.005 |
| 8 | 0.004 | 0.004 | 0.000 |
| 12 | 0.005 | 0.000 | 0.004 |
| 16 | 0.004 | 0.004 | 0.000 |


#### main() function:
Main function is where the all desrired proceeses are set up and executed. It starts by checking that a valid number has been inputed by the user. If it isn't a valid number, the code termminates and prints an error message to enter a valid number. If a valid number is entered, it calls MPI_Init to start the parallel environment, which makes communication between the processes. Each process is given a unique rank and and the total number of processes in the communicator. The function checks to see is the number of processes is is enough for communication, and then determines which task each process should perform; root or client. Finally it calls the MPI_Finalise to shut doen the MPI enviroment safely. 

<img width="195" height="569" alt="Screenshot 2026-04-05 174629" src="https://github.com/user-attachments/assets/4807666c-8942-4da9-a081-8c1c1ae8b367" />


#### check_args():
This function checks that the program is executed with the correct number of command line arguments. The argument is expected to be 2, the programe name +1, if this is the case, it converts the text into a integer using atoi. IF the argument is wrong or missing, it prints an error message and exits the program.

<img width="229" height="334" alt="Screenshot 2026-04-05 174815" src="https://github.com/user-attachments/assets/ce348f55-39a7-4b68-91bc-a43e0ab2d109" />

#### check_uni_size():
This function ensures that the MPI has enough processes to perform the program. It checks that the total number of processes is atleast the minimum required. If its size is too small, it prints an error statement and terminates the code.

#### check_task():
THis function decides which role each process should take based on its rank. If the process has rank 0, it should be designated as the root and will perform the root task. Every other process should act as a client and run the client task.

<img width="316" height="424" alt="Screenshot 2026-04-05 174936" src="https://github.com/user-attachments/assets/4864f015-1f93-4546-9667-851ec2ae56e3" />

#### root_task():
This function is executed by the root process only. It has to wait for eveyone else to finsih before its able to be ran. It collects and combines the results sent by all the client processes by looping over all other rank and recieving message from each one. Each result is added to a running toal and once all messages have been recieved, the root prints the final combiend value.

<img width="243" height="690" alt="Screenshot 2026-04-05 175002" src="https://github.com/user-attachments/assets/128343f1-29e1-43c4-b400-cf97ba6c49eb" />

#### client_task():
This function is exectuded by all processes expect the root. Each client performs a simple calculation based on its rank and inputted argument. The result is sent to the root process.

<img width="213" height="308" alt="Screenshot 2026-04-05 175018" src="https://github.com/user-attachments/assets/fce93c32-5871-4f88-8724-61d73be6f444" />

Next the code was turned into a one-operation mathematical formula that would achieve the same result as the MPI program. The program works by having each client process calculate a value equal to its rank multiplied by the input number(num_args). These values are then sent to the rooprocess, which adds them together to get a final result. Mathenatically this can be shown by by calculating the sum num_arg *(1+2+3+...+(N-1)) where N is the total number of processes.
This can be better written as: num_arg * ((N-1)N)/2.

### Activity 3:
The aim of the vector_serial.c code is to read in a number from the command line, create a vector array of that size and fill it with values from 0 to n-1. It then sums all the elements and prints the result.

#### main function:
This function checks that a valid numerical argument has been inputted. If it has then it dynamically allocates memory for a vector of the same size and sets all values to 0. THe vector is then filled with meaningful data and the program calculates the sum of all elements by calling another function and prints the reults to a screen. The memory is then freed before terminating.

<img width="208" height="622" alt="Screenshot 2026-04-05 175309" src="https://github.com/user-attachments/assets/b0d86c08-ddca-4349-b04f-1edc3d863f16" />

#### check_args():
This function checks that the program recieved the correct number of command line arguments. The argument is expected to be 2, the programe name +1, if this is the case, it converts the text into a integer using atoi. IF the argument is wrong or missing, it prints an error message and exits the program.

<img width="221" height="323" alt="Screenshot 2026-04-05 175333" src="https://github.com/user-attachments/assets/59b73370-e382-4469-9ae0-2f4c0e0e0230" />

#### initialise_vector():
This function sets all elements in a vector to a specified initial value to prepare memory. It iterates through the vector and sets every single slot to the initial value. It helps prevent bad values from interfering with calculations.

<img width="231" height="521" alt="Screenshot 2026-04-05 175344" src="https://github.com/user-attachments/assets/109804bb-c649-4fc4-b06c-2068b117e700" />

#### sum_vector():
This function iterates through the vector and prints each element to the screen. Does this by setting a counter to 0 and then walks through the vector one by one, adding the vlaue of the current element to the running total.

<img width="254" height="644" alt="Screenshot 2026-04-05 175407" src="https://github.com/user-attachments/assets/cdbe0992-88e7-4d5f-85bb-e8fa5765e61a" />

### Activity 4:

### Serial Implementations
| Program | Input Size | Sum | Internal Time (s) | Real Time (s) | User Time (s) | Sys Time (s) |
|--------|------------|-----|-------------------|--------------|--------------|-------------|
| vector_serial | 10 | 0 | N/A | 0.005 | 0.001 | 0.004 |
| vector_serial | 1000 | 0 | N/A | 0.005 | 0.000 | 0.004 |
| vector_serial | 100000000 | 0 | N/A | 0.837 | 0.635 | 0.200 |
| vector_nontrivial | 10 | 285 | 0.000003 | 0.005 | 0.000 | 0.004 |
| vector_nontrivial | 1000 | 332833500 | 0.000014 | 0.004 | 0.000 | 0.004 |
| vector_nontrivial | 100000000 | -1452071552 | 0.564562 | 1.037 | 0.844 | 0.193 |

### MPI Implementations
| Program | Processes | Input Size | Sum | Real Time (s) | User Time (s) | Sys Time (s) |
|--------|----------|------------|-----|--------------|--------------|-------------|
| vector_mpi_benchmarked | 4 | 10 | 285 | 0.453 | 0.138 | 0.166 |
| vector_mpi_benchmarked | 4 | 1000 | 332833500 | 0.443 | 0.125 | 0.178 |
| vector_mpi_benchmarked | 4 | 100000000 | -1452071552 | 0.524 | 0.460 | 0.142 |
| vector_mpi_benchmarked | 2 | 10 | 285 | 0.409 | 0.094 | 0.094 |
| vector_mpi_benchmarked | 2 | 1000 | 332833500 | 0.411 | 0.099 | 0.091 |
| vector_mpi_benchmarked | 2 | 100000000 | -1452071552 | 0.574 | 0.418 | 0.096 |

## Discusion

### Activity 1:
The comparison between MPI and serial versions of the *Hello World* programs show that the serial versions consistently have faster runtimes for simple tasks. While both versions provide the same output, the MPI program takes longer as it has to set up an manage multiplr processes. As more processes are added, the MPI runtime increases slightly due to this extra bit of workload, whilst the serial version runs a simple loop in one process without the extra setup costs. 
Due to this, for small and computationally insignificant tasks, parralleslisation is less efficient and has no benefit, with it being better for problems with higher computational workloads.

### Activity 3:
The benchmarking results shows the difference between serial and parallel versions of the same code. For small input size (e.g. 10), both of the serial codes computted almost instantly with the mpi being quite noticably slower. This is because the workload is too small to benefit from parallel processing and its time is mostly spent on the setting up the MPI enviroment.
For the intermediate size the performance gap remains. The serial code is still neat instantaneous while MPI shows no speed improvement. Its only at the very large input size that the parallel version finally becomes faster than the serial version. This proves that parallel processing works once the workload is heavy enough and can outperform the serial version in terms of real time. Increasing the number of processes furhter reduces the runtime. The improvement from 2 to 4 processes is present but not linear, indicating diminishing returns due to overheads such as process management and communication.
However, the issue of intger overflow was met at the exact point where the parallel code starts to be faster than the serial code. As the calculation of i*i grows very quickly, it exceeds the limits of the standard 32 bit intefer in C. Due to this no input value was found that was large enough tomake the parallel code faster than the serial one without causing an overflow error. While parallelisation provides a clear speed benefit for larger problems, these results show that data types must be chosen carefully to ensure the program remains accuracte while it scales.
