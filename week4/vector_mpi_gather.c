// needed libraries
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int check_args(int argc, char **argv);

int main(int argc, char **argv)
{
    // initialise mpi enviroment
    MPI_Init(&argc, &argv);

    int rank, size;
    // get rank and total number of processes
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // read imput N, upper bound for simulation
    int N = check_args(argc, argv);

    // determine chunk of how going to each process
    int chunk = N / size;
    // compute start and stop indices for the process
    int start = rank * chunk;
    int stop = (rank + 1) * chunk;
    //ensure last process handles any leftover elements
    if (rank == size - 1)
        stop = N;
    //local sum for each process
    int partial_sum = 0;

    // synch all processes before start timiing
    MPI_Barrier(MPI_COMM_WORLD);
    // start timing
    double start_time = MPI_Wtime();
    // compute sum of squares for given range
    for (int i = start; i < stop; i++)
        partial_sum += i * i;

    int *all_sums = NULL;

    if (rank == 0)
        all_sums = malloc(size * sizeof(int));

    MPI_Gather(&partial_sum, 1, MPI_INT,
               all_sums, 1, MPI_INT,
               0, MPI_COMM_WORLD);
    // only root process prints result
    if (rank == 0)
    {
        int final_sum = 0;
 // manually sum all results collected from each process
        for (int i = 0; i < size; i++)
            final_sum += all_sums[i];

        double end_time = MPI_Wtime();
        printf("Final Sum: %d\n", final_sum);
        printf("Time: %f seconds\n", end_time - start_time);
        // clear memory
        free(all_sums);
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
