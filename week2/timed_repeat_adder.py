import sys
import time

def main():
    output = 0

    # checks if there are the right number of arguments
    try:
        # converts the first two arguments to integers
        in_arg = int(sys.argv[1])
        multiple = int(sys.argv[2])
    except:
        raise Exception(
            "Incorrect arguments.\n"
            "Usage: python repeat_adder.py [NUM] [NUM]\n"
            "e.g.\npython repeat_adder.py 2 3"
        )

    # start timing
    start_time = time.perf_counter()

    # iterates over all numbers up to the input
    for i in range(in_arg):
        output = output + multiple

    # end timing
    end_time = time.perf_counter()

    # calculate runtime
    run_time = end_time - start_time

    # prints the output
    print(output)
    print(f"Execution time: {run_time:.6f} seconds")


if __name__ == "__main__":
    main()
