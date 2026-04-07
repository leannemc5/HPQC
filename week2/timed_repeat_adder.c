#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv)
{
    // timing variables
    clock_t start_time, end_time;
    double run_time;

    // start timing
    start_time = clock();

    // creates and initialises the variables
    int i, input, output, multiple;
    i = input = output = multiple = 0;

    // checks if there are the right number of arguments
    if (argc == 3)
    {
        // converts the arguments to integers
        input = atoi(argv[1]);
        multiple = atoi(argv[2]);
    }
    else
    {
        fprintf(stderr, "Incorrect arguments. Usage: repeat_adder [NUM] [NUM]\n");
        fprintf(stderr, "e.g. repeat_adder 2 3\n");
        exit(-1);
    }

    // iterates over all numbers up to input
    for (i = 0; i < input; i++)
    {
        output = output + multiple;
    }

    // end timing
    end_time = clock();

    // calculate runtime
    run_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    // prints the output
    printf("Result: %d\n", output);
    printf("Execution time: %f seconds\n", run_time);

    return 0;
}
