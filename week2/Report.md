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
Both codes can be ran now to see how long it takes each to read in a file and which performs better than the other.
```
./bin/time_file_write data/testfile.txt 30000
python3 week2/time_file_write.py data/testfile.txt 30000
```

## Results:

### Part 1:
real 0m0.006s                     
user 0m0.000s                            
sys 0m0.005s                           

### Part 2:
#### Python script:                                   
Hello World!                   
real    0.035s                 
user    0.023s              
sys     0.008s     

#### C script:   
Hello World!                             
real    0.006s                   
user    0.000s                
sys     0.004s   

### Part 3:

### Part 4:
