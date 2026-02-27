#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int check_args(int argc, char **argv);

/* 
   Custom sum function.
   MPI will call this during reduction.
*/
void my_sum(void *invec, void *inoutvec, int *len, MPI_Datatype *datatype)
{
    int *in = (int *)invec;
    int *inout = (int *)inoutvec;

    for (int i = 0; i < *len; i++)
    {
        inout[i] += in[i];
    }
}

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int N = check_args(argc, argv);

    int chunk = N / size;
    int start = rank * chunk;
    int stop = (rank + 1) * chunk;

    if (rank == size - 1)
        stop = N;

    int partial_sum = 0;
    int final_sum = 0;

    // Create custom MPI operation
    MPI_Op my_op;
    MPI_Op_create(&my_sum, 1, &my_op);

    MPI_Barrier(MPI_COMM_WORLD);
    double start_time = MPI_Wtime();

    for (int i = start; i < stop; i++)
        partial_sum += i * i;

    MPI_Reduce(&partial_sum, &final_sum,
               1, MPI_INT,
               my_op, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double end_time = MPI_Wtime();

    if (rank == 0)
    {
        printf("Final Sum (Custom Reduce): %d\n", final_sum);
        printf("Time: %f seconds\n", end_time - start_time);
    }

    MPI_Op_free(&my_op);
    MPI_Finalize();
    return 0;
}

int check_args(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: mpirun -np [P] ./program [N]\n");
        exit(-1);
    }
    return atoi(argv[1]);
}
