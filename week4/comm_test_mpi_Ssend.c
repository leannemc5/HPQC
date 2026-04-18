// needed libraries
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

// function Prototypes
void root_task(int my_rank, int uni_size);
void client_task(int my_rank, int uni_size);
void check_task(int my_rank, int uni_size);

int main(int argc, char **argv)
{
    // ierror stores the return status of MPI functions (0 is success)
    int ierror = 0;
    int my_rank, uni_size;
    my_rank = uni_size = 0;

    // initialise the mpi execution environment
    ierror = MPI_Init(&argc, &argv);

    // determine the unique ID (rank) and total number of processes (size)
    ierror = MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    ierror = MPI_Comm_size(MPI_COMM_WORLD, &uni_size);

    // logic check: ensure there is more than 1 process to allow communication
    if (uni_size > 1)
    {
        // split specific work based on the process rank
        check_task(my_rank, uni_size);
    }
    else
    {
        // error handling for serial execution
        printf("Unable to communicate with less than 2 processes. Size = %d\n", uni_size);
    }

    // terminate the mpi environment cleanly
    ierror = MPI_Finalize();
    return 0;
}


 // check_task: Acts as a manager to separate the program flow.
 // It ensures Rank 0 acts as the receiver while others act as senders.
 
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


 // root_task: executed only by Rank 0.
 // listens for incoming messages from all other ranks in the communicator.
 
void root_task(int my_rank, int uni_size)
{
    int recv_message, source, count, tag;
    recv_message = source = tag = 0;
    count = 1; // expecting 1 integer per message
    MPI_Status status;

    // loop through every other rank (1 to uni_size-1)
    for (int their_rank = 1; their_rank < uni_size; their_rank++)
    {
        source = their_rank;

        // blocking receive: waits until a message from source arrives
        MPI_Recv(&recv_message, count, MPI_INT, source, tag, MPI_COMM_WORLD, &status);

        printf("Hello, I am %d of %d. Received %d from Rank %d\n",
                my_rank, uni_size, recv_message, source);
    }
}


 // client_task: executed by all ranks except Rank 0.
 // calculates a simple message and sends it to the root process.

void client_task(int my_rank, int uni_size)
{
    int send_message, dest, count, tag;
    send_message = dest = tag = 0;
    count = 1;

    dest = 0; // target for the message is always the root (Rank 0)
    send_message = my_rank * 10; // non trivial calculation for testing

    // blocking send: transfers the local message to the root process
    MPI_Ssend(&send_message, count, MPI_INT, dest, tag, MPI_COMM_WORLD);

    printf("Hello, I am %d of %d. Sent %d to Rank %d\n",
            my_rank, uni_size, send_message, dest);
}
