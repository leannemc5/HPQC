#include <stdio.h>
#include <time.h>


int main()
{
   // printf() displays the string inside quotation
   
   // declare timing variables
   clock_t start_time, end_time;
   double run_time;

   // record start time
   start_time = clock();

   // code to measure
   printf("Hello, World!\n");
   return 0;

   // record end time
   end_time = clock();

   // calculate runtime in seconds
   run_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

   // print runtime
   printf("Execution time: %f seconds\n", run_time);

   return 0;
}
