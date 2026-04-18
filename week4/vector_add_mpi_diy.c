// needed libraires
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

// functions
int check_args(int argc, char **argv);
int sum_vector(int vector[], int size);

int main(int argc, char **argv) {
    // initialise mpi
    MPI_Init(&argc, &argv);
    int rank, size;
    // get size and rank
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    // read imput argurment (size of vector)
    int num_arg = check_args(argc, argv);
    // split how much each vector will handle
    int chunk = num_arg / size;

    //allocates memory for the FULL vector on every process
    int* local_vector = malloc(chunk * sizeof(int));
    int* full_vector = NULL;

    // ONLY Rank 0 makes the meaningful vector
    if (rank == 0) {
        full_vector = malloc(num_arg * sizeof(int));
        for (int i = 0; i < num_arg; i++) {
            *(full_vector + i) = i * i; //pointer math initialisation
        }
    }

    // start timing
    double start = MPI_Wtime();

    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            //diy send using requested pointer logic
            MPI_Send(full_vector + (i * chunk), chunk, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        for (int i = 0; i < chunk; i++) local_vector[i] = full_vector[i];
    } else {
        MPI_Recv(local_vector, chunk, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    // pass pointer to the beginning of this process's segment
    int local_sum = sum_vector(local_vector, chunk);
    int final_sum = 0;
    // reduce all local sums into a final sum on process 0
    MPI_Reduce(&local_sum, &final_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    // end timing
    double end = MPI_Wtime();

    // only root process prints the result
    if (rank == 0) {
        printf("DIY Sum: %d | Time: %f s\n", final_sum, end - start);
        free(full_vector);
    }
    // free memory
    free(local_vector);
    // close mpi
    MPI_Finalize();
    return 0;
}
// function to compute sum of elements in a vector
int sum_vector(int vector[], int size) {
    int sum = 0;
    // iterate through array and accumulate values
    for (int i = 0; i < size; i++) { sum += vector[i]; }
    return sum;
}
// check and parse command line arguements
int check_args(int argc, char **argv) {
    if (argc != 2) exit(-1);
    // convert args from string to int
    return atoi(argv[1]);
}
