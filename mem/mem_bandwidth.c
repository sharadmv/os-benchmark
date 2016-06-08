
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <util.h>
#include <init.h>

char* create_array(unsigned int array_size) {
    return (char*) malloc(array_size * sizeof(unsigned char));
}

double benchmark_write(unsigned int array_size, unsigned int iterations) {
  struct timeval start_time;
  struct timeval end_time;

  double result = 0;
  double num_accesses = 0;
  while (iterations-- > 0) {
    register char* array = create_array(array_size);
    gettimeofday(&start_time, NULL);
    for (int i = 0; i < 8388608; i++) {*(array + (i * 32 + 0) * 4) = 1;
*(array + (i * 32 + 1) * 4) = 1;
*(array + (i * 32 + 2) * 4) = 1;
*(array + (i * 32 + 3) * 4) = 1;
*(array + (i * 32 + 4) * 4) = 1;
*(array + (i * 32 + 5) * 4) = 1;
*(array + (i * 32 + 6) * 4) = 1;
*(array + (i * 32 + 7) * 4) = 1;
*(array + (i * 32 + 8) * 4) = 1;
*(array + (i * 32 + 9) * 4) = 1;
*(array + (i * 32 + 10) * 4) = 1;
*(array + (i * 32 + 11) * 4) = 1;
*(array + (i * 32 + 12) * 4) = 1;
*(array + (i * 32 + 13) * 4) = 1;
*(array + (i * 32 + 14) * 4) = 1;
*(array + (i * 32 + 15) * 4) = 1;
*(array + (i * 32 + 16) * 4) = 1;
*(array + (i * 32 + 17) * 4) = 1;
*(array + (i * 32 + 18) * 4) = 1;
*(array + (i * 32 + 19) * 4) = 1;
*(array + (i * 32 + 20) * 4) = 1;
*(array + (i * 32 + 21) * 4) = 1;
*(array + (i * 32 + 22) * 4) = 1;
*(array + (i * 32 + 23) * 4) = 1;
*(array + (i * 32 + 24) * 4) = 1;
*(array + (i * 32 + 25) * 4) = 1;
*(array + (i * 32 + 26) * 4) = 1;
*(array + (i * 32 + 27) * 4) = 1;
*(array + (i * 32 + 28) * 4) = 1;
*(array + (i * 32 + 29) * 4) = 1;
*(array + (i * 32 + 30) * 4) = 1;
*(array + (i * 32 + 31) * 4) = 1;
};
    gettimeofday(&end_time, NULL);
    num_accesses += array_size / 4;
    result += elapsed(start_time, end_time);
    free(array);
  }
  return result / num_accesses * 1000.0;
}

double benchmark_read(unsigned int array_size, unsigned int iterations) {
  struct timeval start_time;
  struct timeval end_time;

  double result = 0;
  double num_accesses = 0;
  register int sum = 0;
  while (iterations-- > 0) {
    register char* array = create_array(array_size);
    gettimeofday(&start_time, NULL);
    for (int i = 0; i < 8388608; i++) {sum += *(array + (i * 32 + 0) * 4);
sum += *(array + (i * 32 + 1) * 4);
sum += *(array + (i * 32 + 2) * 4);
sum += *(array + (i * 32 + 3) * 4);
sum += *(array + (i * 32 + 4) * 4);
sum += *(array + (i * 32 + 5) * 4);
sum += *(array + (i * 32 + 6) * 4);
sum += *(array + (i * 32 + 7) * 4);
sum += *(array + (i * 32 + 8) * 4);
sum += *(array + (i * 32 + 9) * 4);
sum += *(array + (i * 32 + 10) * 4);
sum += *(array + (i * 32 + 11) * 4);
sum += *(array + (i * 32 + 12) * 4);
sum += *(array + (i * 32 + 13) * 4);
sum += *(array + (i * 32 + 14) * 4);
sum += *(array + (i * 32 + 15) * 4);
sum += *(array + (i * 32 + 16) * 4);
sum += *(array + (i * 32 + 17) * 4);
sum += *(array + (i * 32 + 18) * 4);
sum += *(array + (i * 32 + 19) * 4);
sum += *(array + (i * 32 + 20) * 4);
sum += *(array + (i * 32 + 21) * 4);
sum += *(array + (i * 32 + 22) * 4);
sum += *(array + (i * 32 + 23) * 4);
sum += *(array + (i * 32 + 24) * 4);
sum += *(array + (i * 32 + 25) * 4);
sum += *(array + (i * 32 + 26) * 4);
sum += *(array + (i * 32 + 27) * 4);
sum += *(array + (i * 32 + 28) * 4);
sum += *(array + (i * 32 + 29) * 4);
sum += *(array + (i * 32 + 30) * 4);
sum += *(array + (i * 32 + 31) * 4);
};
    gettimeofday(&end_time, NULL);
    num_accesses += array_size / 4;
    result += elapsed(start_time, end_time);
    free(array);
  }
  return result / num_accesses * 1000.0;
}


int main(){
  unsigned int SIZE = 30;
  initialize();
  double total =  benchmark_read(pow(2, SIZE), 10);
  printf("read,%u,%f\n", (int) pow(2, SIZE), total);
  total =  benchmark_write(pow(2, SIZE), 10);
  printf("write,%u,%f\n", (int) pow(2, SIZE), total);
}

