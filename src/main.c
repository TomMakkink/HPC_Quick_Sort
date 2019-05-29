#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>
#include <stdbool.h>
#include "quicksort.h"

/* Function to print an array */
void printArray(int arr[], int size) 
{ 
    int i; 
    for (i=0; i < size; i++) 
    printf("%d ", arr[i]); 
    printf("\n"); 
} 

bool test(int* arr, int length){ 
    for (int i = 1; i < length; i++){
        if (arr[i] < arr[i-1]) {
            return false;
        } 
    }
    return true; 
}


int main(int argc, char* argv[]){
	int length = 10000; 
    sort(length, argc, argv);
    return 0; 
}

void run_quick_sort_serial(int * arr, int length, int runs, int n){
    printf("Running Serial QuickSort...\n");
    double start, end; 
    
    start = omp_get_wtime(); 
    for (int p = 0; p < runs; p++){
        quickSortSerial(arr, 0, n-1);
    }
    end = omp_get_wtime();
    double time_taken = (end - start) / runs;

    // printArray(arr, length);

    printf("Serial Quicksort took %f10 seconds to execute with array length of %d \n", time_taken, length);   
    if (test(arr, length)) printf("Test Passed \n");
    else printf("Testing failed. Array was not sorted correctly \n");
}

void run_quick_sort_parallel(int * arr, int length, int runs, int n){
    printf("Running Parallel QuickSort...\n");
    double start, end; 
    
    start = omp_get_wtime(); 
    for (int p = 0; p < runs; p++){
        quickSortParallel(arr, 0, n-1);
    }
    end = omp_get_wtime();
    double time_taken = (end - start) / runs;


    printf("Parallel Quicksort took %f10 seconds to execute with array length of %d \n", time_taken, length);   
    if (test(arr, length)) printf("Test Passed \n");
    else printf("Testing failed. Array was not sorted correctly \n");
}

void run_openmp_psrs_sort(int * arr, int length, double runs, int n){
    printf("Running OpenMP Parallel Sort with Regular Sampling...\n");
    double start, end; 
    
    start = omp_get_wtime(); 
    for (int p = 0; p < runs; p++){
        psrs_sort(arr, n);
    }
    end = omp_get_wtime();
    double time_taken = (end - start) / runs;
    printf("OpenMP Parallel took %f10 seconds to execute with array length of %d \n", time_taken, length);   
    if (test(arr, length)) printf("Test Passed \n");
    else printf("Testing failed. Array was not sorted correctly \n");
}


void sort(int length, int argc, char* argv[]){
    int runs = 10; 
    
    int serial_array[length];
    int openmp_array[length];
    int parallel_array[length];

    srand(time(0));
    for (int i = 0; i < length; i++){   
        int randval = rand() % 100; 
        serial_array[i]   = randval;
        openmp_array[i]   = randval; 
        parallel_array[i] = randval; 
    }
    int n = sizeof(serial_array)/sizeof(serial_array[0]);

    run_quick_sort_serial(serial_array, length, runs, n);

    run_quick_sort_parallel(parallel_array, length, runs, n);
    
    run_openmp_psrs_sort(openmp_array, length, runs, n);

    // MPI_Init(&argc,&argv);  
    // psrs_mpi(mpi_array, length);

    return 0; 
}





