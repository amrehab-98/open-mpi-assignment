to Compile the file:
mpicc -O -o output assignment3.c

to Run:
mpirun -n <number_of_processes> ./output

Example:
mpirun -n 6 ./output

Test:
mpirun -n 6  ./output
Enter number of terms: 1000
Enter value of x: 2
Parallel runtime is 0.000116 seconds
Sequential runtime is 0.000003 seconds