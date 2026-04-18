// needed libraries
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

// functions
int check_args(int argc, char **argv);
void initialise_vector(int vector[], int size, int initial);
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
    int* my_vector = malloc(num_arg * sizeof(int));
    initialise_vector(my_vector, num_arg, 0);

    // ONLY Rank 0 makes the meaningful vector
    if (rank == 0) {
        for (int i = 0; i < num_arg; i++) {
            my_vector[i] = i * i;
        }
    }
    // start timing
    double start = MPI_Wtime();

    //broadcast the finished vector
    MPI_Bcast(my_vector, num_arg, MPI_INT, 0, MPI_COMM_WORLD);

    //each process sums its specific part of the shared vector
    int start_idx = rank * chunk;
    // pass pointer to the beginning of this process's segment
    int local_sum = sum_vector(&my_vector[start_idx], chunk);

    int final_sum = 0;
    // reduce all local sums into a final sum on process 0
    MPI_Reduce(&local_sum, &final_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // end timing
    double end = MPI_Wtime();

    // only root process prints the result
    if (rank == 0) {
        printf("Bcast Sum: %d | Time: %f s\n", final_sum, end - start);
    }
    // free memory
    free(my_vector);
    // close mpi
    MPI_Finalize();
    return 0;
}
// function to compute sum of elements in a vector
int sum_vector(int vector[], int size) {
    int sum = 0;
    // iterate through the array and accumulate values
    for (int i = 0; i < size; i++) { sum += vector[i]; }
    return sum;
}
// function to initialize all elements of a vector to a given value
void initialise_vector(int vector[], int size, int initial) {
    for (int i = 0; i < size; i++) { vector[i] = initial; }
}
// check and parse command line arguements
int check_args(int argc, char **argv) {
    if (argc != 2) exit(-1);
    // convert args from strong to integer
    return atoi(argv[1]);
}
