#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/time.h>
#include "util.h"
#include "init.h"

pthread_t tid[1];

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

void* context_switch2(void* n_iterations) {
  int result;
  unsigned int n_iters = *((int*) n_iterations);
  struct timeval start_time;
  struct timeval end_time;

  for (int i = 0; i < n_iters; i++) {
    sched_yield();
  }
}

int main() {
  initialize();
  unsigned int NUM_TRIALS = 1000;
  unsigned int NUM_ITERATIONS = 10000;
  int sched = sched_getscheduler(getpid());
  pthread_setschedprio(pthread_self(), sched_get_priority_max(sched));

  printf("Number of trials: %u\n", NUM_TRIALS);
  printf("Iterations per trial: %u\n", NUM_ITERATIONS);

  double context_switch_time = 0.0;
  for (int i = 0; i < NUM_TRIALS; i++) {
    pthread_create(&(tid[0]), NULL, &context_switch2, &NUM_ITERATIONS);
    context_switch_time += context_switch(NUM_ITERATIONS);
    pthread_join(tid[0], NULL);
  }
  printf("Elapsed time for thread_switch(): %f ms\n", context_switch_time / NUM_TRIALS / NUM_ITERATIONS);
}
