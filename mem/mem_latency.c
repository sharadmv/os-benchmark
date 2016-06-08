#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <util.h>
#include <init.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

void create_array(unsigned int array_size, unsigned int stride_size, char** array_pointer, char*** stride_pointer) {
    char* array = (char*) malloc(array_size * sizeof(unsigned char));
    *array_pointer = array;
    unsigned int num_strides = array_size / stride_size;
    char** p;
    for (int i = 0; i < num_strides; i++) {
      unsigned int index = i * stride_size;
      unsigned int next_index = ((i + 1) % num_strides) * stride_size;
      p = (char**) (array + index);
      *p = (char*) (array + next_index);
    }
    *stride_pointer = (char**) array;
}

#define ONE p = (char **)*p;
#define FIVE ONE ONE ONE ONE ONE
#define TEN FIVE FIVE
#define FIFTY TEN TEN TEN TEN TEN
#define HUNDRED FIFTY FIFTY

double benchmark(unsigned int array_size, unsigned int stride_size, unsigned int num_accesses) {
  char* array;
  char** strides;
  create_array(array_size, stride_size, &array, &strides);

  char** p = strides;

  unsigned int iterations = num_accesses;

  struct timeval start_time;
  struct timeval end_time;

  char dummy;

  gettimeofday(&start_time, NULL);
  while (iterations -- > 0) {
    HUNDRED;
  }
  gettimeofday(&end_time, NULL);
  free(array);
  double result = (double) elapsed(start_time, end_time);
  double num_a = (double) num_accesses;
  return result * 1000.0 / (num_a * 100);
}


int main(){
  int MIN_SIZE = 12, MAX_SIZE = 30;
  int MIN_STRIDE = 10, MAX_STRIDE = 12;
  initialize();
  for (int size = MIN_SIZE; size <= MAX_SIZE; size++) {
    for (int stride = MIN_STRIDE; stride <= MIN(size, MAX_STRIDE); stride++) {
      double total =  benchmark((int)pow(2, size), (int)pow(2, stride), 100000);
      printf("%u,%u,%f\n", (int) pow(2, size), (int)pow(2, stride), total);
    }
  }
}

