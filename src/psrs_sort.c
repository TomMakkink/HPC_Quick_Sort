#include "omp.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"


/* determine the boundaries for the sublists of an local array */
void calc_partition_borders(int array[],    // array being sorted
                            int start,
                            int end,              // separate the array into current process range
                            int result[],
                            int at,               // this process start point in result
                            int pivots[],   // the pivot values
                            int first_pv,         // first pivot
                            int last_pv)          // last pivot
{
  int mid, lowerbound, upperbound, center;
  int pv;

  mid = (first_pv + last_pv) / 2;
  pv = pivots[mid-1];
  lowerbound = start;
  upperbound = end;
  while(lowerbound <= upperbound) {
    center = (lowerbound + upperbound) / 2;
    if(array[center] > pv) {
      upperbound = center - 1;
    } else {
      lowerbound = center + 1;
    }
  }
  result[at + mid] = lowerbound;

  if(first_pv < mid) {
    calc_partition_borders(array, start, lowerbound - 1, result, at, pivots, first_pv, mid - 1);
  }
  if(mid < last_pv) {
    calc_partition_borders(array, lowerbound, end, result, at, pivots, mid + 1, last_pv);
  }
}


/* Compare ints (lifted from the harness) */
int lcompare(const void * ptr2num1, const void * ptr2num2)
{
  int num1 = *((int*) ptr2num1);
  int num2 = *((int*) ptr2num2);

  if ( num1 > num2 )
    return 1;
  else if ( num1 < num2 )
    return -1;
  else
    return 0;
}


/*
  Sort a portion of an array
  @todo: see if merge sort might be better in some cases
*/
void sortll(int *a, int len)
{
  qsort(a, len, sizeof(int), lcompare);
}


/* sort an array in non-descending order */
void psrs_sort(int *a, int n) {
  if(n > 1){
  // Testing shows that our algorithm is now the quickest 
  int p, size, rsize, sample_size;
  int *sample, *pivots;
  int *partition_borders, *bucket_sizes, *result_positions;
  int **loc_a_ptrs;

  // Determine the appropriate number of threads to use
  // p^3 <= n - We need this to hold true
  p = omp_get_max_threads();
  p = p*p*p;
  if(p > n){
	p = floor(pow(n,0.33));
	p-=p%2;
  }else{
   p = omp_get_max_threads();
   p-=p%2;
  }
  omp_set_num_threads(p);

  size  = (n + p - 1) / p;
  rsize = (size + p - 1) / p;
  sample_size = p * (p - 1);

  loc_a_ptrs = malloc(p * sizeof(int *));
  sample = malloc(sample_size * sizeof(int));
  partition_borders = malloc(p * (p + 1) * sizeof(int));
  bucket_sizes = malloc(p * sizeof(int));
  result_positions = malloc(p * sizeof(int));
  pivots = malloc((p - 1) * sizeof(int));

  #pragma omp parallel
  {
    int i, j, max, thread_num, start, end, loc_size, offset, this_result_size;
    int *loc_a, *this_result, *current_a;

    thread_num = omp_get_thread_num();
    start = thread_num * size;
    end = start + size - 1;
    if(end >= n) end = n - 1;
    loc_size = (end - start + 1);
    end = end % size;

    loc_a = malloc(loc_size * sizeof(int));
    memcpy(loc_a, a + start, loc_size * sizeof(int));
    loc_a_ptrs[thread_num] = loc_a;

    sortll(loc_a, loc_size); // Testing shows that this sequential sort is quickest in this instance
	
    offset = thread_num * (p - 1) - 1;

    for(i = 1; i < p; i++) {
      if(i * rsize <= end) {
        sample[offset + i] = loc_a[i * rsize - 1];
      } else {
        sample[offset + i] = loc_a[end];
      }
    }

    #pragma omp barrier

    offset = thread_num * (p + 1);
    partition_borders[offset] = 0;
    partition_borders[offset + p] = end + 1;
    calc_partition_borders(loc_a, 0, loc_size-1, partition_borders, offset, pivots, 1, p-1);

    #pragma omp barrier

    max = p * (p + 1);
    bucket_sizes[thread_num] = 0;
    for(i = thread_num; i < max; i += p + 1) {
      bucket_sizes[thread_num] += partition_borders[i + 1] - partition_borders[i];
    }

    #pragma omp barrier

    #pragma omp single
    {
      result_positions[0] = 0;
      for(i = 1; i < p; i++) {
        result_positions[i] = bucket_sizes[i-1] + result_positions[i-1];
      }
    }

    #pragma omp barrier

    this_result = a + result_positions[thread_num];

    if(thread_num == p-1) {
      this_result_size = n - result_positions[thread_num];
    } else {
      this_result_size = result_positions[thread_num+1] - result_positions[thread_num];
    }

    // pluck this threads sublist from each of the local arrays
    this_result = a + result_positions[thread_num];

    for(i = 0, j = 0; i < p; i++) {
      int low, high, partition_size;
      offset = i * (p + 1) + thread_num;
      low = partition_borders[offset];
      high = partition_borders[offset+1];
      partition_size = (high - low);
      if(partition_size > 0) {
        memcpy(this_result+j, &(loc_a_ptrs[i][low]), partition_size * sizeof(int));
        j += partition_size;
      }
    }

    // sort p local sorted arrays
    sortll(this_result, this_result_size); // Testing shows that this sequential sort is quickest in this instance
    
    #pragma omp barrier
    free(loc_a);
  }

  free(loc_a_ptrs);
  free(sample);
  free(partition_borders);
  free(bucket_sizes);
  free(result_positions);
  free(pivots);
  }
}

