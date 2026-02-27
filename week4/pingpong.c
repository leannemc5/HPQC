#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int check_args(int argc, char **argv);
void run_pingpong(int my_rank, int num_pings);

int main(int argc, char **argv)
{
    int ierror = 0;
    int my_rank, uni_size;

    // 1. Check arguments before MPI Init
    int num_pings = check_args(argc, argv);

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &uni_size);

    // 2. Safety check: This benchmark requires exactly 2 processes
    if (uni_size != 2)
    {
        if (my_rank == 0) 
            fprintf(stderr, "Error: Ping-pong requires exactly 2 processes.\n");
        MPI_Finalize();
        return -1;
    }

    // 3. Run the benchmark
    run_pingpong(my_rank, num_pings);

    MPI_Finalize();
    return 0;
}

void run_pingpong(int my_rank, int num_pings)
{
    int counter = 0;
    int tag = 0;
    int count = 1;
    double start_time, end_time;
    MPI_Status status;

    // Synchronize processes before starting the timer
    MPI_Barrier(MPI_COMM_WORLD);
    start_time = MPI_Wtime();

    while (counter < num_pings)
    {
        if (my_rank == 0)
        {
            // Root sends (Ping)
            MPI_Send(&counter, count, MPI_INT, 1, tag, MPI_COMM_WORLD);
            // Root receives (Pong)
            MPI_Recv(&counter, count, MPI_INT, 1, tag, MPI_COMM_WORLD, &status);
        }
        else if (my_rank == 1)
        {
            // Client receives (Ping)
            MPI_Recv(&counter, count, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
            // Client increments
            counter++;
            // Client sends (Pong)
            MPI_Send(&counter, count, MPI_INT, 0, tag, MPI_COMM_WORLD);
        }
    }

    end_time = MPI_Wtime();

    // 4. Output results from Root
    if (my_rank == 0)
    {
        double elapsed_time = end_time - start_time;
        // Average time per "trip" (one ping + one pong)
        double avg_time = elapsed_time / num_pings;

        printf("Final Counter: %d\n", counter);
        printf("Total Elapsed Time: %f s\n", elapsed_time);
        printf("Average Time per Round-Trip: %f s\n", avg_time);
    }
}

int check_args(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: mpirun -np 2 pingpong [NUM_PINGS]\n");
        exit(-1);
    }
    return atoi(argv[1]);
}
