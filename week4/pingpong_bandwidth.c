// needed libraries
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int check_args(int argc, char **argv, int *num_pings, int *num_elements);

int main(int argc, char **argv) {
    int num_pings, num_elements;
    // check args
    if (check_args(argc, argv, &num_pings, &num_elements) != 0) return -1;

    // initialize mpi environment
    MPI_Init(&argc, &argv);
    int my_rank, uni_size;
    // get rank and total number of process
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &uni_size);

    // ensure only 2 processes are used
    if (uni_size != 2) {
        if (my_rank == 0) fprintf(stderr, "Error: Requires 2 processes.\n");
        MPI_Finalize();
        return -1;
    }

    // allocate memory for payload
    int *payload = (int *)malloc(num_elements * sizeof(int));
    //check if allocation worked
    if (payload == NULL) {
        MPI_Finalize();
        return -1;
    }

    // initialise array (using element 0 as the counter)
    for (int i = 0; i < num_elements; i++) payload[i] = 0;

    //record start time
    double start_time = MPI_Wtime();

    // loop until counter reaches num_pings
    while (payload[0] < num_pings) {
        if (my_rank == 0) {
            // process 0 send payload to process 1
            MPI_Send(payload, num_elements, MPI_INT, 1, 0, MPI_COMM_WORLD);
            //p0 receives payload back from p1
            MPI_Recv(payload, num_elements, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        } else {
            // p1 receives payload from p0
            MPI_Recv(payload, num_elements, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            payload[0]++; // Increment the counter hidden in the first element
            //send updated payload to p0
            MPI_Send(payload, num_elements, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }
    // calculate total elapsed time
    double elapsed_time = MPI_Wtime() - start_time;

    // only p0 computes and prints result
    if (my_rank == 0) {
        // avg time for 1 round trip
        double avg_round_trip = elapsed_time / num_pings;
        // avg time for 1/2 round trip
        double one_way_time = avg_round_trip / 2.0;
        //total message size in bytes
        size_t bytes = num_elements * sizeof(int);
        
        // bandwidth calculation: size of data / time, converts bytes to Mib 
        double bandwidth_MBs = (bytes / (1024.0 * 1024.0)) / one_way_time;

        printf("Size: %zu Bytes | Avg One-Way: %f s | Bandwidth: %f MiB/s\n", 
                bytes, one_way_time, bandwidth_MBs);
    }
    // free memory
    free(payload);
    //close mpi
    MPI_Finalize();
    return 0;
}
// check and parse input arguments
int check_args(int argc, char **argv, int *num_pings, int *num_elements) {
    if (argc != 3) { 
        fprintf(stderr, "Usage: mpirun -np 2 bandwidth [PINGS] [ELEMENTS]\n");
        return -1;
    }
    // converts args from string to int
    *num_pings = atoi(argv[1]);
    *num_elements = atoi(argv[2]);
    return 0;
}
