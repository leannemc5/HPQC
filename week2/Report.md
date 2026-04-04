# Overview of Week 2 activities
Each activity studied how long it takes C and Python programs to run using differnt operations including simple print statments, loops and I/O file.
This was done using the time command for external benchmarking and time_print programs for internal benchmarking.

## Instructions
### Activity 1:
The first activity was to become familiar with the built-in time command called time. 
This was done by timing the command ls: time ls

### Activity 2:
The next activity involved timing how long it takes to run a simple 'Hello World' C and Python programs.
After cloning the class materials to my repo, in the home directory a bin file was created to run the files.
Next the C programm was compiled using:                                                       
```gcc week2/hello_world.c -o bin/hello_world```                                                                  
From this both programs can be ran using:
```
time python3 week2/hello_world.py 
time ./bin/hello_world
```

The process was repeated but for 'Repeat Adder' program. This code is just a long winded way of multiplying 2 numbers; m and n; by constantly adding m to itself n times.
The C program was compiled using gcc ```week2/repeat_adder.c -o bin/repeat_adder```.
Using the same commands from earlier, the times for the programs to be ran can be found.
``` 
time python3 week2/repeat_adder.py n m
time ./bin/repeat_adder n m
```

Note: Both programs had to be edited before being able to run them. 
The given code had a mixed of space and tab intents which cheetah did not like which had to be manually fixed before complinging each program.

### Activity 3:
This activities focus was on introducing internal benchmarking. A sample code in python and C called *'time_print'* were provided in the class repo with the internal timing code already included.
Similarily to previous steps, the C code was compiled via ```gcc week2/time_print.c -o bin/time_print```.
Both codes were then using various different inputs to test how long it takes to run the code in each language.
```
./bin/time_print 50000
python3 week2/time_print.py 50000

```
### Activity 4: 
The last exercise wished for us to take the provided C and Python templates in this weeks repo and use them as a guide to input I/O to see how long it takes to read and write to a file.
Two codes were made, with one focusing on timing how long it took to write to a file, and the other focused on how long it took to read from a file.

#### Writing to a file:
The *time_print.c code* renamed to *time_write.c* and was modified to redirect the core loop output from the console to a text file called *output.txt* inside the 'dataweek2' folder.
This was done by adding a file point and used *fopen* to tell the computer where to write the data.
```
FILE *fp;
fp = fopen("dataweek2/output.txt", "w");
```
To ensure it was working correctly, a test if statment was made. If the check failed, the program would terminate and a error message would be printed to the sreen. 
```
if (fp == NULL)
	{
		fprintf(stderr, "Error opening file.\n");
		exit(-1);
	}
```
Inside the loop of the code, the *printf* statement was changed to *fprintf* to ensure the numerical outputs would print to the file and finally added a fclose statment to make make the file close and save.

For the python version, the file was named *time_write.py* and the results are saved into a file by opening a file using the *with open()* statement to target *dataweek2/output.txt* in write mode *w*. With was used as it manually opens and closes the file once the loop is terminated.
```
with open('dataweek2/output.txt', 'w') as f:
```
Inside the loop , the print function with *f.write()* which sends the numbers to a file buffer.
```
f.write("{}, ".format(i))
```

When both codes were completed, the C code was compiled similarly to before and both codes are ran to see how long it takes for each to run
```
gcc week2/time_file_write.c -o bin/time_file_write

./bin/time_file_write data/testfile.txt 30000
python3 week2/time_file_write.py data/testfile.txt 30000
```

#### Read from a file:
The *time_print.c code* renamed to *time_read.py* and was modified to read in data from a file instead of creating some.
First, the file *output.txt* was opened using *r* the command to read a file. 
```
FILE *fp = fopen("dataweek2/output.txt", "r");
if (fp == NULL)	{
fprintf(stderr, "couldn't open file .\n");
                exit(-1);
```
To determine how big the file was, *fseek* was used to go through the end of the file and *ftell* to measure its exact size. This was done to enusre that the file wasnt too large or the program would crash and to use memory effectively. 
```
fseek(fp, 0, SEEK_END);
file_size = ftell(fp);
fseek(fp, 0, SEEK_SET);
```
Once the size of the file was found, dynamic memory allocation *malloc* was used to create a buffer/ storage space that can contain the file.
```
buffer = malloc(file_size + 1);
```
*Fseek* was used again to return to the start of the code for it to read through from the beginning and fread to copy the entire file into memory beofre clsong the file and calculating how long the process took.
```
fread(buffer, 1, file_size, fp);
fclose(fp);
```
Once these changes were implmented, the code was compiled
```
gcc week2/time_read.c -o bin/time_read
```

The python script was renamed to *time_read.py* to read in a file into a python script.
The for loop was replaced with a *with open()* function set to *r* to acess the file in the *dataweek2* file. 
```
with open("dataweek2/output.txt", "r") as f:
data = f.read()
```
Both codes were then ran to see how long it takes each to read in a file and which performs better than the other.
```
time ./bin/time_read 5000
time python3 week2/time_read.py 5000
```

## Results:

### Part 1:
Running ls, returned this list of these folders as expected.
*bin   dataweek2  PHY1090            vector_mpi_reduce         vector_mpi_send
data  HPQC       vector_mpi_gather  vector_mpi_reduce_custom*

The system was able to do this in the following time
| Category | Time |
| --- | --- |
| real | 0m0.006s |
| user | 0m0.000s |
| sys | 0m0.005s |
### Part 2:
#### Hello World scritps:                                   
After running *hello_world.py*, it printed out `Hello World!`    
| Category | Time |
| --- | --- |
|real  |  0.035s  |               
|user  |  0.023s |             
|sys   |  0.008s|     
  
After running *hello_world.c*, it printed out `Hello World!`   
| Category | Time |
| --- | --- |
|real  |  0.006s |                  
|user  |  0.000s  |              
|sys  |   0.004s  |

#### Repeat Adder scripts: 
| Source | Inputs | Solution | Real Time | User Time | Sys Time |
| :---: | :---: | :---: | :---: | :---: | :---: |
| `repeat_adder.py` | 2, 3 | 6 | 0.034s | 0.027s | 0.004s |
| `repeat_adder.py` | 4, 5 | 20 | 0.034s | 0.026s | 0.004s |
| `repeat_adder.py` | 41098, 54209 | 11314089 | 0.036s | 0.023s | 0.009s|
| `repeat_adder.py` | 5823, 1943 | 11314089 | 0.036s | 0.027s | 0.004s|
| `repeat_adder.c` | 2, 3 | 6 | 0.007s | 0.004s | 0.001s |
| `repeat_adder.c` | 2, 3 | 6 | 0.005s | 0.004s | 0.000s |
| `repeat_adder.c` | 41098, 54209 | -2067085814 | 0.006s | 0.000s | 0.004s|
| `repeat_adder.c` | 5823, 1943 | 11314089 | 0.004s | 0.000s | 0.004s|

### Part 3:
When this code is ran, it printed out a sequential list of all the integers starting from 0 up to the user prompted number.
| Source | Input | Runtime for core loop | Real Time | User Time | Sys Time |
| :---: | :---: | :---: | :---: | :---: | :---: |
| `time_print.c` | 10 | 1.566969s | 0.437s | 0.137s | 0.063s |
| `time_print.c` | 100 |  0.000169s | 0.006s | 0.000s | 0.005s |
| `time_print.c` | 1000 | 0.000365s | 0.004s | 0.000s | 0.004s |
| `time_print.c` | 10000 | 0.002727s | 0.006s | 0.006s | 0.000s |
| `time_print.py` | 10 | 1.5974044799804688e-05s | 0.039s | 0.024s | 0.008s |
| `time_print.py` | 100 | 6.651878356933594e-05s | 0.032s | 0.027s | 0.004s |
| `time_print.py` | 1000 | 0.000649s | 0.032s | 0.026s | 0.005s |
| `time_print.py` | 10000 | 0.006794s | 0.039s | 0.024s | 0.012s |

### Part 4:
#### Write file:
| Source | Input | Runtime for core loop | Real Time | User Time | Sys Time |
| :---: | :---: | :---: | :---: | :---: | :---: |
| `time_write.py` | 1000 | 0.003458s | 0.039s | 0.023s | 0.008s |
| `time_write.py` | 5000 | 0.003492s | 0.035s | 0.027s | 0.004s |
| `time_write.py` | 90000 | 0.34918s | 0.068s | 0.051s | 0.004s |
| `time_write.c` | 1000 | 0.000176s | 0.010s | 0.000s | 0.005s |
| `time_write.c` | 5000 | 0.000808s | 0.009s | 0.000s | 0.005s |
| `time_write.c` | 90000 | 0.009817s | 0.025s | 0.014s | 0.000s |

#### Read file:
| Source | Input | Runtime for core loop | Real Time | User Time | Sys Time |
| :---: | :---: | :---: | :---: | :---: | :---: |
| `time_read.py` | 1000 | 0.000116348s | 0.032s | 0.015s | 0.016s | 
| `time_read.py` | 5000 | 0.000119686s | 0.033s | 0.028s | 0.005s |  
| `time_read.py` | 90000 | 0.000308275s | 0.032s | 0.017s | 0.013s | 
| `time_read.c` | 1000 | 0.001375s | 0.007s | 0.005s | 0.001 s | 
| `time_read.c` | 5000 | 0.000857s | 0.006s | 0.000s | 0.004s | 
| `time_read.c` | 90000 | 0.000302s | 0.004s | 0.000s | 0.004s |  

## Discussion:
The experiments explored how the runtimes of C and Python programs differ across different types of operations. Using both Linux time command and internal timing functions, it can be seen how each language behaves with increasing workloads.

For the *Hello World* programs, the C code ran significantly faster than the Python code. However, the complete runtime for both programs was extremely small, which shows that for trivial tasks the difference in performance is negligible and either language is suitable. The observed difference is largely due to Python being an interpreted language with an additional startup overhead where C programs are compiled direclty to machine code, making it faster.

In the *Repeat Adder* programs, the difference between both programs becomes more noticeable. For small input values, the runtime remain similar as the computation is done almost instantly, however as the input size is increased, the C program scaled much better than Python. This is due to that fact that python doesn't use fixed sized integers like C. Python stores numbers as big, flexible objects that can grow as large as needed, whilst C only stores numbers to about 7 decimal places. This makes Python's values more accurate but comes at the expense of computational timing.
Due to C programs less flexible integer size, it gives integer overflow for very large inputs( greater than 2^31 -1), producing incorrect negative results.

When examining the internal timing functions on the *time_print* functions, it can be seen that both programs had a unexpected output for very small inputs. It can be seen that the small inputs appeared to take longer to run than the larger ones. This is due to measurement noise and limited resolution of timing finctions for very short runtimes. 
Excluding these outliers, the runtime increased with input size and Python runtimes were sligthly slower than C for complete runtime and for runtime of core loop. 

For the file I/O operations, both languages showed an increase in runtime as the input size grew, but with writing to file being generally slower than reading due to the additional overhead of disk operations. Once again, C performed faster than Python especially for larger file sizes. This again is due to its lower overhead. 
However for the runtime for the core loop, in some cases Python was much faster than C. This is likely due to differences in how timing was measured and how each language handles I/O operations internally. In Python, file operations are highly optimised and often buffered, meaning that the data may have be written to memory first and only later flushed to disk. This can make the measured loop appear faster, as the actual disk writing is deferred, while in contrast, C programs perform file operations more directly, causing the timing of the loop to include more of the actual I/O cost.

Overall, it can be seen that C is generally faster than Python as C excels at raw computational power while python prioritises ease of use and flexibilty. C gives better performance for computation heavy and large scale tasks due to its compiled nature and efficient memory handling while python offers greater flexibility, simplicity, and no integer overflow, but at the cost of increased runtime. For small scale tasks Python is a good choice, however, for high performance computing or  where execution time is critical, C is the better choice.
