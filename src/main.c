#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>
#include <stdbool.h>
#include "quicksort-omp.h"

int lenArr = 1000000; 
int numthreads = 8;

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
    sort(lenArr);
    return 0; 
}

void run_quick_sort_serial(int * arr){
    printf("\nRunning Serial QuickSort...\n"); fflush(stdout);
    double start, end; 
    start = omp_get_wtime(); 
    quickSortSerial(arr, 0, lenArr-1);
    end = omp_get_wtime();
    double time_taken = end - start;

    printf("Serial Quicksort took %f10 seconds to execute with array length of %d \n", time_taken, lenArr);   
    if (test(arr, lenArr)) printf("Test Passed \n");
    else printf("Testing failed. Array was not sorted correctly \n");
}

void run_quick_sort_parallel(int * arr){
    printf("\nRunning Parallel QuickSort...\n");
    double start, end; 
    start = omp_get_wtime(); 
    quickSort_parallel(arr, lenArr, numthreads);
    end = omp_get_wtime();
    double time_taken = end - start;

    printf("Parallel Quicksort took %f10 seconds to execute with array length of %d \n", time_taken, lenArr);   
    if (test(arr, lenArr)) printf("Test Passed \n");
    else printf("Testing failed. Array was not sorted correctly \n");
}

void run_openmp_psrs_sort(int * arr){
    printf("\nRunning OpenMP Parallel Sort with Regular Sampling...\n");
    double start, end; 
    
    start = omp_get_wtime(); 
    psrs_sort(arr, lenArr);
    end = omp_get_wtime();
    double time_taken = end - start;
    printf("OpenMP Parallel took %f10 seconds to execute with array length of %d \n", time_taken, lenArr);   
    if (test(arr, lenArr)) printf("Test Passed \n");
    else printf("Testing failed. Array was not sorted correctly \n");
}


void sort(int length){
    int minMum = 1;
	int maxNum = lenArr;

	int* serialQSortArray;
	int* parallelQSortArray; 
	int* PRSPQSortArray;
	serialQSortArray = (int*) malloc(lenArr*sizeof(int));
	parallelQSortArray = (int*) malloc(lenArr*sizeof(int));
	PRSPQSortArray = (int*) malloc(lenArr*sizeof(int));
	
	printf("\nSize of the array (aprox.): %lu MBytes \n", (lenArr*sizeof(int)/(1024*1024)) );
	printf("TOTAL MEMORY ALLOCATED:  3 x array = (aprox.): %lu MBytes \n\n", (lenArr*sizeof(int)/(1024*1024))*3 );  
	
	// Initialise the array with random numbers
	int i;
	srand(time(0)); // seed
	printf("Initializing the arrays with random numbers...\n");
	for (i=0; i<lenArr; i++){
		// RAND_MAX = 2147483647 = INT_MAX 
		// printf("RAND_MAX %u \n", RAND_MAX );
		serialQSortArray[i] = minMum+(rand()%maxNum);
		parallelQSortArray[i] = serialQSortArray[i];
		PRSPQSortArray[i] = serialQSortArray[i];
		//printf("%d \n", arr1[i] ); 
	}
	printf("Initialization complete\n");
    
    
    run_quick_sort_serial(serialQSortArray);

    run_quick_sort_parallel(parallelQSortArray);
    
    run_openmp_psrs_sort(PRSPQSortArray);

    return 0; 
}





