#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int check_args(int argc, char **argv, int *num_pings, int *num_elements);

int main(int argc, char **argv) {
    int num_pings, num_elements;
    // Check args: [PINGS] [NUM_ELEMENTS]
    if (check_args(argc, argv, &num_pings, &num_elements) != 0) return -1;

    MPI_Init(&argc, &argv);
    int my_rank, uni_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &uni_size);

    if (uni_size != 2) {
        if (my_rank == 0) fprintf(stderr, "Error: Requires 2 processes.\n");
        MPI_Finalize();
        return -1;
    }

    // Allocate the array (vector) based on the size argument
    int *payload = (int *)malloc(num_elements * sizeof(int));
    if (payload == NULL) {
        MPI_Finalize();
        return -1;
    }

    // Initialise array (using element 0 as the counter)
    for (int i = 0; i < num_elements; i++) payload[i] = 0;

    double start_time = MPI_Wtime();

    while (payload[0] < num_pings) {
        if (my_rank == 0) {
            MPI_Send(payload, num_elements, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(payload, num_elements, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        } else {
            MPI_Recv(payload, num_elements, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            payload[0]++; // Increment the counter hidden in the first element
            MPI_Send(payload, num_elements, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    double elapsed_time = MPI_Wtime() - start_time;

    if (my_rank == 0) {
        double avg_round_trip = elapsed_time / num_pings;
        double one_way_time = avg_round_trip / 2.0;
        size_t bytes = num_elements * sizeof(int);
        
        // Bandwidth calculation: Data Size / Time
        double bandwidth_MBs = (bytes / (1024.0 * 1024.0)) / one_way_time;

        printf("Size: %zu Bytes | Avg One-Way: %f s | Bandwidth: %f MiB/s\n", 
                bytes, one_way_time, bandwidth_MBs);
    }

    free(payload);
    MPI_Finalize();
    return 0;
}

int check_args(int argc, char **argv, int *num_pings, int *num_elements) {
    if (argc != 3) {
        fprintf(stderr, "Usage: mpirun -np 2 bandwidth [PINGS] [ELEMENTS]\n");
        return -1;
    }
    *num_pings = atoi(argv[1]);
    *num_elements = atoi(argv[2]);
    return 0;
}
