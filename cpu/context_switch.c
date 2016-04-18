#include <sys/time.h>
#include <pthread.h>
#include <stdio.h>
#include <sched.h>
#include <unistd.h>
#include "util.h"
#include "init.h"

float context_switch(unsigned int n_iterations) {
  int result;
  struct timeval start_time;
  struct timeval end_time;

  result = gettimeofday(&start_time, NULL);
  for (int i = 0; i < n_iterations; i++) {
    sched_yield();
  }
  result = gettimeofday(&end_time, NULL);
  return elapsed(start_time, end_time) / 1000.0;
}

int main() {
  initialize();
  fork();
  unsigned int NUM_TRIALS = 100;
  unsigned int NUM_ITERATIONS = 10000;
  int sched = sched_getscheduler(getpid());
  pthread_setschedprio(pthread_self(), sched_get_priority_max(sched));
  printf("Number of trials: %u\n", NUM_TRIALS);
  printf("Iterations per trial: %u\n", NUM_ITERATIONS);

  double context_switch_time = 0.0;
  for (int i = 0; i < NUM_TRIALS; i++) {
    context_switch_time += context_switch(NUM_ITERATIONS);
  }
  printf("Elapsed time for context_switch(): %f ms\n", context_switch_time / NUM_TRIALS / NUM_ITERATIONS);
}
