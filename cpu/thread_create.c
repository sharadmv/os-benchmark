#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/time.h>
#include "util.h"
#include "init.h"


pthread_t tid[1001];
void* foo() {
}

float measure_create(unsigned int n_iterations) {
  int result;
  struct timeval start_time;
  struct timeval end_time;

  unsigned long long int x;
  unsigned long long int y;
  double sum = 0;
  unsigned int lo,hi;
  for (int i = 0; i < n_iterations; i++) {
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    x = ((long long)hi << 32) | lo;
    pthread_create(&(tid[i]), NULL, &foo, NULL);
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    y = ((long long)hi << 32) | lo;
    pthread_join(tid[i], NULL);
    sum += y - x;
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
    context_switch_time += measure_create(NUM_ITERATIONS);
  }
  printf("Number of cycles for thread_switch(): %f cycles\n", context_switch_time / NUM_TRIALS);
}
