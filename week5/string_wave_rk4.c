// import needed libraries
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// declares the functions that will be called within main
// note how declaration lines are similar to the initial line
// of a function definition, but with a semicolon at the end;
int check_args(int argc, char **argv);
void initialise_vector(double vector[], int size, double initial);
int generate_timestamps(double* time_stamps, int time_steps, double step_size);
double driver(double time);
void print_header(FILE** p_out_file, int points);

// rk4 update
void update_positions(double* pos, double* vel, int points, double time, double dt);

int main(int argc, char **argv)
{
// declare and initialise the numerical argument variable
    int points = check_args(argc, argv);
  // creates variables for the vibration
    int cycles = 5; // number of cycles to show
    int samples = 25; // sampling rate in samples per cycle
    int time_steps = cycles * samples + 1; // total timesteps
    double step_size = 1.0/samples;

// creates a vector for the time stamps in the data
    double* time_stamps = malloc(time_steps * sizeof(double));
    initialise_vector(time_stamps, time_steps, 0.0);
    generate_timestamps(time_stamps, time_steps, step_size);

// creates a vector variable for the current positions
    double* positions = malloc(points * sizeof(double));
    initialise_vector(positions, points, 0.0);
// creates a vector variable for the current velocity
    double* velocity = malloc(points * sizeof(double));
// creates a vector variable for the current positions
    initialise_vector(velocity, points, 0.0);

// creates file
    FILE* out_file = fopen("HPQC/week5/string_wave_rk4_HELP.csv","w");
    print_header(&out_file, points);

// iterates through each time step in collection
    for (int i = 0; i < time_steps; i++)
    {
// updates the position using a function
        update_positions(positions, velocity, points, time_stamps[i], step_size);

// prints an index and time stamp
        fprintf(out_file, "%d, %lf", i, time_stamps[i]);
// iterates over all of the points on the line
        for (int j = 0; j < points; j++)
            fprintf(out_file, ", %lf", positions[j]);
// prints a new line
        fprintf(out_file, "\n");
    }

// if we use malloc, must free when done!
    free(time_stamps);
    free(positions);
    free(velocity);
    fclose(out_file);

    return 0;
}

// rk4 code
void update_positions(double* pos, double* vel, int n, double time, double dt)
{
// set values for wave number and mass
    double k = 2.0;
    double m = 2.0;

    // make RK4 arrays
    double *k1 = malloc(n*sizeof(double));
    double *k2 = malloc(n*sizeof(double));
    double *k3 = malloc(n*sizeof(double));
    double *k4 = malloc(n*sizeof(double));

    double *temp = malloc(n*sizeof(double));

    // k1
    pos[0] = driver(time);
    vel[0] = driver(time);
    for(int i=0;i<n;i++)
    {
        k1[i] = vel[i];
    }

    // k2
    for(int i=0;i<n;i++)
    {
        temp[i] = pos[i] + 0.5*dt*k1[i];
    }

    for(int i=0;i<n;i++)
    {
        k2[i] = vel[i];
    }

    // k3
    for(int i=0;i<n;i++)
    {
        temp[i] = pos[i] + 0.5*dt*k2[i];
    }

    for(int i=0;i<n;i++)
    {
        k3[i] = vel[i];
    }

    // k4
    for(int i=0;i<n;i++)
    {
        temp[i] = pos[i] + dt*k3[i];
    }

    for(int i=0;i<n;i++)
    {
        k4[i] = vel[i];
    }

    // final update
    for(int i=0;i<n;i++)
    {
        pos[i] += (dt/6.0)*(k1[i] + 2*k2[i] + 2*k3[i] + k4[i]);
    }

    // free memory
    free(k1); free(k2); free(k3); free(k4);
    free(temp);
}

double driver(double time)
{
    return sin(time * 2.0 * M_PI);
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

// defines a set of timestamps
int generate_timestamps(double* timestamps, int time_steps, double step_size)
{
        for (int i = 0; i < time_steps ; i++)
        {
                timestamps[i]=i*step_size;
        }
        return time_steps;
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

        // returns the sum
        return sum;
}

// defines a function to initialise all values in a vector to a given inital value
void initialise_vector(double vector[], int size, double initial)
{
        // iterates through the vector
        for (int i = 0; i < size; i++)
        {
                // sets the elements of the vector to the initial value
                vector[i] = initial;
        }
}

// defines a function to print a vector of ints
void print_vector(double vector[], int size)
{
        // iterates through the vector
        for (int i = 0; i < size; i++)
        {
                // prints the elements of the vector to the screen
                printf("%d, %lf\n", i, vector[i]);
        }
}

// defines a function that checks your arguments to make sure they'll do what you need
int check_args(int argc, char **argv)
{
        // declare and initialise the numerical argument
        int num_arg = 0;

        // check the number of arguments
        if (argc == 2) // program name and numerical argument
        {
                // declare and initialise the numerical argument
                num_arg = atoi(argv[1]);
        }
        else // the number of arguments is incorrect
        {
                // raise an error
                fprintf(stderr, "ERROR: You did not provide a numerical argument!\n");
                fprintf(stderr, "Correct use: %s [NUMBER]\n", argv[0]);

                // and exit COMPLETELY
                exit (-1);
        }
        return num_arg;
}
