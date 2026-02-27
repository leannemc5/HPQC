#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int check_args(int argc, char **argv);
int sum_vector(int vector[], int size);

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int num_arg = check_args(argc, argv);
    int chunk = num_arg / size;

    int* local_vector = malloc(chunk * sizeof(int));
    int* full_vector = NULL;

    if (rank == 0) {
        full_vector = malloc(num_arg * sizeof(int));
        for (int i = 0; i < num_arg; i++) {
            full_vector[i] = i * i;
        }
    }

    double start = MPI_Wtime();

    //scatter handles the distribution automatically
    MPI_Scatter(full_vector, chunk, MPI_INT, local_vector, chunk, MPI_INT, 0, MPI_COMM_WORLD);

    int local_sum = sum_vector(local_vector, chunk);
    int final_sum = 0;
    MPI_Reduce(&local_sum, &final_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    double end = MPI_Wtime();

    if (rank == 0) {
        printf("Scatter Sum: %d | Time: %f s\n", final_sum, end - start);
        free(full_vector);
    }

    free(local_vector);
    MPI_Finalize();
    return 0;
}

int sum_vector(int vector[], int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) { sum += vector[i]; }
    return sum;
}

int check_args(int argc, char **argv) {
    if (argc != 2) exit(-1);
    return atoi(argv[1]);
}
