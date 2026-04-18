// needed libraries
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int check_args(int argc, char **argv);

int main(int argc, char **argv)
{
    // initialise mpi enviroment
    MPI_Init(&argc, &argv);
    // get rank and total number of processes
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    // read imput N, upper bound for simulation
    int N = check_args(argc, argv);
    // determine chunk of how going to each process
    int chunk = N / size;
    int start = rank * chunk;
    int stop = (rank + 1) * chunk;
    //ensure last process handles any leftover elements
    if (rank == size - 1)
        stop = N;
    //local sum for each process
    int partial_sum = 0;

    // synch all processes before start timiing
    MPI_Barrier(MPI_COMM_WORLD);
    // recors start time
    double start_time = MPI_Wtime();
    // compute sum of squares for given range
    for (int i = start; i < stop; i++)
        partial_sum += i * i;
    // if not root process
    if (rank != 0)
    {
        // send computed sum to root process
        MPI_Send(&partial_sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    else
    {
        // root process starts with its own sum
        int final_sum = partial_sum;
        int received_sum; // temp variable to store incoming sums
        // receive sums from other processes
        for (int source = 1; source < size; source++)
        {
            MPI_Recv(&received_sum, 1, MPI_INT,
                     source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            // accumulate into final result
            final_sum += received_sum;
        }

        double end_time = MPI_Wtime();
        printf("Final Sum: %d\n", final_sum);
        printf("Time: %f seconds\n", end_time - start_time);
    }
    // close mpi enviroment
    MPI_Finalize();
    return 0;
}
// check command line inputs
int check_args(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: mpirun -np [P] ./program [N]\n");
        exit(-1);
    }
     //convert args to integer and return
    return atoi(argv[1]);
}
