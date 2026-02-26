#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

/* --- Function Prototypes --- */
void root_task(int my_rank, int uni_size);
void client_task(int my_rank, int uni_size);
void check_task(int my_rank, int uni_size);

int main(int argc, char **argv)
{
    // ierror stores the return status of MPI functions (0 is success)
    int ierror = 0;
    int my_rank, uni_size;
    my_rank = uni_size = 0;

    // Initialise the MPI execution environment
    ierror = MPI_Init(&argc, &argv);

    // Determine the unique ID (rank) and total number of processes (size)
    ierror = MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    ierror = MPI_Comm_size(MPI_COMM_WORLD, &uni_size);

    // Basic logic check: ensure there is more than 1 process to allow communication
    if (uni_size > 1)
    {
        // Delegate specific work based on the process rank
        check_task(my_rank, uni_size);
    }
    else
    {
        // Error handling for serial execution (single process)
        printf("Unable to communicate with less than 2 processes. Size = %d\n", uni_size);
    }

    // Terminate the MPI environment cleanly
    ierror = MPI_Finalize();
    return 0;
}

/*
 * check_task: Acts as a manager to separate the program flow.
 * It ensures Rank 0 acts as the receiver while others act as senders.
 */
void check_task(int my_rank, int uni_size)
{
    if (0 == my_rank)
    {
        root_task(my_rank, uni_size);
    }
    else
    {
        client_task(my_rank, uni_size);
    }
}

/*
 * root_task: Executed only by Rank 0.
 * Listens for incoming messages from all other ranks in the communicator.
 */
void root_task(int my_rank, int uni_size)
{
    int recv_message, source, count, tag;
    recv_message = source = tag = 0;
    count = 1; // Expecting 1 integer per message
    MPI_Status status;

     double start_time, end_time;

    // Loop through every other rank (1 to uni_size-1)
    for (int their_rank = 1; their_rank < uni_size; their_rank++)
    {
        source = their_rank;

        start_time = MPI_Wtime();
        // Blocking receive: waits until a message from 'source' arrives
        MPI_Recv(&recv_message, count, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
        end_time = MPI_Wtime();
        printf("Hello, I am %d of %d. Received %d from rank %d (Took %fs)\n",
                my_rank, uni_size, recv_message, source, end_time - start_time);
    }
}

/*
 * client_task: Executed by all ranks except Rank 0.
 * Calculates a simple message and sends it to the root process.
 */
void client_task(int my_rank, int uni_size)
{
    double start_time, end_time;
    int send_message, dest, count, tag;
    send_message = dest = tag = 0;
    count = 1;

    dest = 0; // The target for the message is always the root (Rank 0)
    send_message = my_rank * 10; // Non-trivial calculation for testing

    start_time = MPI_Wtime();

    // Blocking send: transfers the local message to the root process
    MPI_Send(&send_message, count, MPI_INT, dest, tag, MPI_COMM_WORLD);
    end_time = MPI_Wtime();

    printf("Hello, I am %d of %d. Sent %d (Took %fs)\n",
            my_rank, uni_size, send_message, end_time - start_time);
}
