#include "stdio.h"
#include "mpi.h"
#include "time.h"

long double factorial(long double n){

    if(n == 0)
        return 1.0;

    long double result = 1;
    for(long double i=1; i<=n; i++){
        result *=i;
    }
    return result;
}
long double calculate_numerator(long double x, long long counter){
    int sign;
    if(counter % 2 == 0){
        sign = 1;
    }
    else
    {
        sign = -1;
    }

    long double result = 1;
    for(int i=1; i<=counter; i++){
        result *= (x*x);
    }
    result*=sign;
    return result;
}
long double sequentialCode(long double x, long double acc){
    long double value = 0;
    for (long double i = 0; i < acc; i++) {
        value += (calculate_numerator(x,i) / factorial(2*i));
    }
    return value;
}
long double parallel_code(long double x, int acc, int rank, int num_processes){
    long double value = 0;
    for (int i = rank; i < acc; i+=num_processes) {
        value += (calculate_numerator(x,i) / factorial(2*i));
    }
    return value;
}
int main(int argc, char** argv){
    // long double res = sequentialCode(x, acc);
    long double acc;
    int num_processes;
    int rank;
    long double x;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    setbuf(stdout, NULL);
    if(rank == 0){
        printf("enter number of terms: ");
        scanf("%Lf",&acc);
        printf("enter value of x: ");
        scanf("%Lf",&x);
    }
    
    MPI_Bcast(&acc, 1, MPI_LONG_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&x, 1, MPI_LONG_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    double time1 = MPI_Wtime();    
    
    long double local_result =  parallel_code(x, acc, rank, num_processes);
    long double global_result;
    MPI_Reduce(&local_result, &global_result, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if(rank == 0){
       printf("result is = %.100Lf\n",global_result);
    }
    double time2 = MPI_Wtime();
    double duration = time2 - time1;
    double global;
    // Get the max time value from all processes to print
    MPI_Reduce(&duration, &global, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    if (rank == 0)
    {
        printf("Parallel runtime is %f seconds\n", global);
        time1 = MPI_Wtime();
        long double seqResult = sequentialCode(x, acc);
        time2 = MPI_Wtime();
        duration = time2-time1;
        printf("Sequential runtime is %f seconds\n", duration);
    }
    // Finalize the MPI environment.
    MPI_Finalize();

}
