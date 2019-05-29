// Sourced from: https://www.geeksforgeeks.org/quick-sort/ and 
// https://github.com/eduardlopez/quicksort-parallel/blob/master/quicksort-omp.h
#include "quicksort.h"
#include <stdio.h> 
#define OMP_CUTOFF 100

  
// A utility function to swap two elements 
void swap(int* a, int* b) 
{ 
    int t = *a; 
    *a = *b; 
    *b = t; 
} 

  
/* This function takes last element as pivot, places 
   the pivot element at its correct position in sorted 
    array, and places all smaller (smaller than pivot) 
   to left of pivot and all greater elements to right 
   of pivot */
int partition(int arr [], int low, int high) 
{ 
    int pivot = arr[high];    // pivot 
    int i = (low - 1);        // Index of smaller element 
  
    for (int j = low; j <= high- 1; j++) 
    { 
        // If current element is smaller than or 
        // equal to pivot 
        if (arr[j] <= pivot) 
        { 
            i++;    // increment index of smaller element 
            swap(&arr[i], &arr[j]); 
        } 
    } 
    swap(&arr[i + 1], &arr[high]); 
    return (i + 1); 
} 
  
/* The main function that implements QuickSort 
 arr[] --> Array to be sorted, 
  low  --> Starting index, 
  high  --> Ending index */
void quickSortSerial(int arr [], int low, int high) 
{ 
    if (low < high) 
    { 
        /* pi is partitioning index, arr[p] is now 
           at right place */
        int pi = partition(arr, low, high); 
  
        // Separately sort elements before 
        // partition and after partition 
        quickSortSerial(arr, low, pi - 1); 
        quickSortSerial(arr, pi + 1, high); 
    }
} 

void quickSortParallelRun(int arr [], int low, int high){
    if (low < high){
        int pi = partition(arr, low, high); 

        if ((high - low) < OMP_CUTOFF){       
            quickSortParallelRun(arr, low, pi - 1); 
            quickSortParallelRun(arr, pi + 1, high);       
        } else if (high - low > OMP_CUTOFF) {      
            #pragma omp task 
            quickSortParallelRun(arr, low, pi - 1);
            #pragma omp task
            quickSortParallelRun(arr, pi + 1, high);
        }
    }
}

void quickSortParallel(int arr [], int low, int high){

    #pragma omp parallel
    {
        #pragma omp single nowait
        quickSortParallelRun(arr, low, high);
    }
}
  