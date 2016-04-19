#include <sys/time.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>
#include "util.h"
#include "init.h"

float measure_fork(unsigned int n_iterations) {
  int result;
  struct timeval start_time;
  struct timeval end_time;
  int cpid;

  unsigned long long int x;
  unsigned long long int y;
  double sum = 0;
  unsigned int lo,hi;
  for (int i = 0; i < n_iterations; i++) {
    result = gettimeofday(&start_time, NULL);
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    x = ((long long)hi << 32) | lo;
    cpid = fork();
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    y = ((long long)hi << 32) | lo;
    sum += y - x;
    if (cpid == 0) {
      exit(0);
    }
  }

  return sum / n_iterations;
}

int main() {
  initialize();
  unsigned int NUM_TRIALS = 100;
  unsigned int NUM_ITERATIONS = 1000;
  int sched = sched_getscheduler(getpid());
  pthread_setschedprio(pthread_self(), sched_get_priority_max(sched));

  printf("Number of trials: %u\n", NUM_TRIALS);
  printf("Iterations per trial: %u\n", NUM_ITERATIONS);

  double context_switch_time = 0.0;
  for (int i = 0; i < NUM_TRIALS; i++) {
    context_switch_time += measure_fork(NUM_ITERATIONS);
  }
  printf("Elapsed cycles for context_switch(): %f cycles\n", context_switch_time / NUM_TRIALS);
}
