#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int check_args(int argc, char **argv);
void initialise_vector(int vector[], int size, int initial);
int sum_vector(int vector[], int size);

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int num_arg = check_args(argc, argv);
    int chunk = num_arg / size;

    //allocates the FULL vector
    int* my_vector = malloc(num_arg * sizeof(int));
    initialise_vector(my_vector, num_arg, 0);

    // ONLY Rank 0 makes the meaningful vector
    if (rank == 0) {
        for (int i = 0; i < num_arg; i++) {
            my_vector[i] = i * i;
        }
    }

    double start = MPI_Wtime();

    //broadcast the finished vector
    MPI_Bcast(my_vector, num_arg, MPI_INT, 0, MPI_COMM_WORLD);

    //each process sums its specific part of the shared vector
    int start_idx = rank * chunk;
    int local_sum = sum_vector(&my_vector[start_idx], chunk);

    int final_sum = 0;
    MPI_Reduce(&local_sum, &final_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    double end = MPI_Wtime();

    if (rank == 0) {
        printf("Bcast Sum: %d | Time: %f s\n", final_sum, end - start);
    }

    free(my_vector);
    MPI_Finalize();
    return 0;
}

int sum_vector(int vector[], int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) { sum += vector[i]; }
    return sum;
}

void initialise_vector(int vector[], int size, int initial) {
    for (int i = 0; i < size; i++) { vector[i] = initial; }
}

int check_args(int argc, char **argv) {
    if (argc != 2) exit(-1);
    return atoi(argv[1]);
}
