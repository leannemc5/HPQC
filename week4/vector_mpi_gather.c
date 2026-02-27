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

    int *all_sums = NULL;

    if (rank == 0)
        all_sums = malloc(size * sizeof(int));

    MPI_Gather(&partial_sum, 1, MPI_INT,
               all_sums, 1, MPI_INT,
               0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        int final_sum = 0;

        for (int i = 0; i < size; i++)
            final_sum += all_sums[i];

        double end_time = MPI_Wtime();

        printf("Final Sum: %d\n", final_sum);
        printf("Time: %f seconds\n", end_time - start_time);

        free(all_sums);
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
