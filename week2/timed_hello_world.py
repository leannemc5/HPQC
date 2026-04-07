import time

#record start time
start_time = time.time()

#code to measure
print("Hello, World!")

#record end time
end_time = time.time()

#calculate runtime
run_time = end_time - start_time

print(f"Execution time: {run_time:.6f} seconds")
