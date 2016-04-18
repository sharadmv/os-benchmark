#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include "util.h"
#include "init.h"

double measure_measure(int n_iterations) {
  int result;
  struct timeval wat;

  unsigned long long int x;
  unsigned long long int y;
  double sum = 0;
  unsigned int lo,hi;
  for (int i = 0; i < n_iterations; i++) {
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    x = ((long long)hi << 32) | lo;
    result = gettimeofday(&wat, NULL);
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    y = ((long long)hi << 32) | lo;
    sum += y - x;
  }
  return sum / n_iterations;
}

double measure_measure2(int n_iterations) {
  int result;
  struct timeval start_time, end_time, wat;

  unsigned long long int x;
  unsigned long long int y;
  double sum = 0;
  unsigned int lo,hi;
  result = gettimeofday(&start_time, NULL);
  __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
  x = ((long long)hi << 32) | lo;
  for (int i = 0; i < n_iterations; i++) {
    result = gettimeofday(&start_time, NULL);
  }
  __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
  y = ((long long) hi << 32) | lo;
  result = gettimeofday(&end_time, NULL);
  printf("%lu\n", elapsed(start_time, end_time));
  sum = y - x;
  return sum;
}

int main() {
  initialize();
  double cycles = measure_measure(10000000);
  printf("Average number of cycles: %f\n", cycles);
  printf("Average time between time measurement: %f\n", cycles / 3500000000.0);
  double cycles2 = measure_measure2(100000000);
  printf("Average number of cycles: %f\n", cycles2);
  printf("Average time between time measurement: %f\n", cycles2 / 3500000000.0);
}
