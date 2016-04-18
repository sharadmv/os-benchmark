#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>

int foo() {
  return 10;
}

int foo2() {
  getpid();
  return 10;
}

long elapsed(struct timeval start, struct timeval end) {
    long secs = end.tv_sec - start.tv_sec;
    long usecs = end.tv_usec - start.tv_usec;
    return secs * 1000000 + usecs;
}

float measure_getpid(int n_iterations, int (*func)()) {
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
  int NUM_TRIALS = 100;
  int NUM_ITERATIONS = 10000000;

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

