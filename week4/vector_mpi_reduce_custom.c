// needed libraries
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int check_args(int argc, char **argv);

//Custom sum function.
//mpi will call this during reduction.
void my_sum(void *invec, void *inoutvec, int *len, MPI_Datatype *datatype)
{
    int *in = (int *)invec; // incoming data
    int *inout = (int *)inoutvec; // accumulated resukt
    // loop over all elements in arrays, *len = how many elements to process
    for (int i = 0; i < *len; i++)
    {
       //calculate sum
       //store result in inout
       inout[i] += in[i];
    }
}

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
   //final reduced sum
    int final_sum = 0;

    // create custom MPI operation
    MPI_Op my_op;
    MPI_Op_create(&my_sum, 1, &my_op);

    // synch all processes before start timiing
    MPI_Barrier(MPI_COMM_WORLD);
    // start timing
    double start_time = MPI_Wtime();
   // compute sum of squares for given range
    for (int i = start; i < stop; i++)
        partial_sum += i * i;

   // reduce all sums into final_aum on p0
    // mpi_sum specifies summation across all processes
    MPI_Reduce(&partial_sum, &final_sum,
               1, MPI_INT,
               my_op, 0, MPI_COMM_WORLD);
   
    MPI_Barrier(MPI_COMM_WORLD);
    double end_time = MPI_Wtime();

   // only root process prints result
    if (rank == 0)
    {
        printf("Final Sum (Custom Reduce): %d\n", final_sum);
        printf("Time: %f seconds\n", end_time - start_time);
    }
    // free memory
    MPI_Op_free(&my_op);
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
