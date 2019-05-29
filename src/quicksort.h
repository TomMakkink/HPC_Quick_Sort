#ifndef QUICKSORT_H
#define QUICKSORT_H

void swap(int* left, int* right);
int partition(int arr [], int low, int high);
void quickSortSerial(int arr [], int low, int high);

int lcompare(const void * ptr2num1, const void * ptr2num2);
void calc_partition_borders(int array[],
               int start,
               int end,
               int sublist_sizes[],
               int at,
               int pivots[],
               int first_p,
               int last_p);
void psrs_sort(int *a, int n);
void sortll(int *a, int len);

#endif
