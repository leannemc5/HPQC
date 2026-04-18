//import needed libraries
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

// declares the functions that will be called within main
// note how declaration lines are similar to the initial line
// of a function definition, but with a semicolon at the end;
int check_args(int argc, char **argv);
void initialise_vector(double vector[], int size, double initial);
void print_vector(double vector[], int size);
int sum_vector(int vector[], int size);
int generate_timestamps(double* time_stamps, int time_steps, double step_size);
double driver(double time);
void print_header(FILE** p_out_file, int points);

int main(int argc, char **argv)
{
    // MPI setup
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // check arguments
    if (argc != 5)
    {
        if (rank == 0)
        {
            fprintf(stderr, "Usage: %s <points> <cycles> <samples> <output_file>\n", argv[0]);
        }
        MPI_Finalize();
        return -1;
    }

    int points = atoi(argv[1]);
    int cycles = atoi(argv[2]);
    int samples = atoi(argv[3]);
    char* filename = argv[4];

    // ensure even division
    if (points % size != 0)
    {
        if (rank == 0)
            fprintf(stderr, "ERROR: points must be divisible by number of processes\n");
        MPI_Finalize();
        return -1;
    }

    int local_points = points / size;

    // simulate parameters
    int time_steps = cycles * samples + 1;
    double step_size = 1.0 / samples;

    // timestamps
    double* time_stamps = (double*) malloc(time_steps * sizeof(double));
    initialise_vector(time_stamps, time_steps, 0.0);
    generate_timestamps(time_stamps, time_steps, step_size);

    // local positions
    double* positions = (double*) malloc(local_points * sizeof(double));
    initialise_vector(positions, local_points, 0.0);

    double* new_positions = (double*) malloc(local_points * sizeof(double));
    double left_value;

    // file -> only root writes
    FILE* out_file = NULL;
    if (rank == 0)
    {
        out_file = fopen(filename, "w");
        print_header(&out_file, points);
    }

    // main time loop
    for (int i = 0; i < time_steps; i++)
    {
        double time = time_stamps[i];

        // communicate with neighbours
        if (rank > 0)
        {
            MPI_Recv(&left_value, 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        if (rank < size - 1)
        {
            MPI_Send(&positions[local_points - 1], 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD);
        }

        // update positions (same logic as original update_positions) 
        if (rank == 0)
            new_positions[0] = driver(time);
        else
            new_positions[0] = left_value;

        for (int j = 1; j < local_points; j++)
        {
            new_positions[j] = positions[j - 1];
        }

        for (int j = 0; j < local_points; j++)
        {
            positions[j] = new_positions[j];
        }

        // aggregate results
        double* all_positions = NULL;
        if (rank == 0)
            all_positions = (double*) malloc(points * sizeof(double));

        MPI_Gather(positions, local_points, MPI_DOUBLE,
                   all_positions, local_points, MPI_DOUBLE,
                   0, MPI_COMM_WORLD);

        // write to file -> only root
        if (rank == 0)
        {
            fprintf(out_file, "%d, %lf", i, time);

            for (int j = 0; j < points; j++)
            {
                fprintf(out_file, ", %lf", all_positions[j]);
            }

            fprintf(out_file, "\n");
            free(all_positions);
        }
    }

    // clean up. used malloc must free when done!
    free(time_stamps);
    free(positions);
    free(new_positions);

    if (rank == 0)
        fclose(out_file);

    // close MPI
    MPI_Finalize();
    return 0;
}

// prints a header to the file
// double-pointer used to allow this function to move the file pointer
void print_header(FILE** p_out_file, int points)
{
    fprintf(*p_out_file, "#, time");
    for (int j = 0; j < points; j++)
    {
        fprintf(*p_out_file, ", y[%d]", j);
    }
    fprintf(*p_out_file, "\n");
}


// defines a simple harmonic oscillator as the driving force
double driver(double time)
{
    double value = sin(time * 2.0 * M_PI);
    return value;
}


// defines a set of timestamps
int generate_timestamps(double* timestamps, int time_steps, double step_size)
{
    for (int i = 0; i < time_steps; i++)
    {
        timestamps[i] = i * step_size;
    }
    return time_steps;
}


// defines a function to initialise all values in a vector to a given inital value
void initialise_vector(double vector[], int size, double initial)
{
    for (int i = 0; i < size; i++)
    {
// sets the elements of the vector to the initial value
        vector[i] = initial;
    }
}


// defines a function to sum a vector of ints into another int
int sum_vector(int vector[], int size)
{
 // creates a variable to hold the sum
    int sum = 0;
// iterates through the vector
    for (int i = 0; i < size; i++)
    {
// sets the elements of the vector to the initial value
        sum += vector[i];
    }
    return sum;
}

// defines a function to print a vector of ints
void print_vector(double vector[], int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%d, %lf\n", i, vector[i]);
    }
}

// defines a function that checks your arguments to make sure they'll do what you need
int check_args(int argc, char **argv)
{
    int num_arg = 0;

// check the number of arguments
    if (argc == 2) // program name and numerical argument
    {
 // declare and initialise the numerical argument
        num_arg = atoi(argv[1]);
    }
    else // the number of arguments is incorrect
    {
        fprintf(stderr, "ERROR: You did not provide a numerical argument!\n");
        fprintf(stderr, "Correct use: %s [NUMBER]\n", argv[0]);
        exit(-1);
    }

    return num_arg;
}
