#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    //initialise mpi
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc !=2)
    {
        if (rank == 0) fprintf(stderr, "Usage: mpirun -np [N] %s [VECTOR_SIZE]\n", argv[0]);
        MPI_Finalize();
        return -1;
    }
    int total_elements = atoi(argv[1]);

    //peusdo code
    int chunk = total_elements / size;
    int start = rank * chunk;
    int stop = (rank + 1) * chunk;

    if (rank == size -1) stop = total_elements;

    int partial_sum = 0;
    for (int i = start; i < stop; i++)
    {
        partial_sum += i * i;
    }

    // using mpi_int
    if (rank != 0)
    {
        MPI_Send(&partial_sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    else{
        int final_sum = partial_sum;
        int received_sum = 0;

        for (int source = 1; source < size; source++){
           MPI_Recv(&received_sum, 1, MPI_INT, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
           final_sum += received_sum;
        }

        printf("Parallel Sum: %d\n", final_sum);
    }
    MPI_Finalize();
    return 0;
}

// defines a function that checks your arguments to make sure they'll do what you need
int check_args(int argc, char **argv)
{
        // declare and initialise the numerical argument
        int num_arg = 0;

        // check the number of arguments
        if (argc == 2) // program name and numerical argument
        {
                // declare and initialise the numerical argument
                num_arg = atoi(argv[1]);
        }
        else // the number of arguments is incorrect
        {
                // raise an error
                fprintf(stderr, "ERROR: You did not provide a numerical argument!\n");
                fprintf(stderr, "Correct use: %s [NUMBER]\n", argv[0]);

                // and exit COMPLETELY
                exit (-1);
        }
        return num_arg;
}
