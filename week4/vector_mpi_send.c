#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int check_args(int argc, char **argv);

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

    MPI_Barrier(MPI_COMM_WORLD);
    double start_time = MPI_Wtime();

    for (int i = start; i < stop; i++)
        partial_sum += i * i;

    if (rank != 0)
    {
        MPI_Send(&partial_sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    else
    {
        int final_sum = partial_sum;
        int received_sum;

        for (int source = 1; source < size; source++)
        {
            MPI_Recv(&received_sum, 1, MPI_INT,
                     source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            final_sum += received_sum;
        }

        double end_time = MPI_Wtime();
        printf("Final Sum: %d\n", final_sum);
        printf("Time: %f seconds\n", end_time - start_time);
    }

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
