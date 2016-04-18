#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include "util.h"
#include "init.h"

int foo() {
  return 1;
}

int foo2() {
  getpid();
  return 1;
}

float measure_getpid(unsigned int n_iterations, int (*func)()) {
  int result;
  struct timeval start_time;
  struct timeval end_time;

  result = gettimeofday(&start_time, NULL);
  for (int i = 0; i < n_iterations; i++) {
    func();
  }
  result = gettimeofday(&end_time, NULL);
  return elapsed(start_time, end_time) / 1000.0;
}

int main(){
  initialize();
  unsigned int NUM_TRIALS = 100;
  unsigned int NUM_ITERATIONS = 1000000;
  printf("Number of trials: %u\n", NUM_TRIALS);
  printf("Iterations per trial: %u\n", NUM_ITERATIONS);

  double getpid_time = 0.0;
  double foo_time = 0.0;
  double foo2_time = 0.0;
  for (int i = 0; i < NUM_TRIALS; i++) {
    getpid_time += measure_getpid(NUM_ITERATIONS, &getpid);
    foo_time += measure_getpid(NUM_ITERATIONS, &foo);
    foo2_time += measure_getpid(NUM_ITERATIONS, &foo2);
  }
  printf("Elapsed time for getpid(): %f ms\n", getpid_time / NUM_TRIALS);
  printf("Elapsed time for foo(): %f ms\n", foo_time / NUM_TRIALS);
  printf("Elapsed time for foo2(): %f ms\n", foo2_time / NUM_TRIALS);
}

