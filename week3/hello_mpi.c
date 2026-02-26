#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    int myRank, uniSize, ierror;
    // sets up MPI
    ierror = MPI_Init(&argc, &argv);
    // gets the rank and  world size
    ierror = MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    ierror = MPI_Comm_size(MPI_COMM_WORLD, &uniSize);
    // print to the screen
    printf("Hello World!, I am %d of %d\n",myRank, uniSize);
    // close all MPI connections
    ierror = MPI_Finalize();
    return 0;
}
