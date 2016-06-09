#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include "util.h"
#include "init.h"

int foo0() {
  return 1;
}

int foo1(int a) {
  return 1;
}

int foo2(int a, int b) {
  return 1;
}

int foo3(int a, int b, int c) {
  return 1;
}

int foo4(int a, int b, int c, int d) {
  return 1;
}

int foo5(int a, int b, int c, int d, int e) {
  return 1;
}

int foo6(int a, int b, int c, int d, int e, int f) {
  return 1;
}

int foo7(int a, int b, int c, int d, int e, int f, int g) {
  return 1;
}


float measure_foo0(int n_iterations) {
  int result;
  struct timeval start_time;
  struct timeval end_time;

  result = gettimeofday(&start_time, NULL);
  for (int i = 0; i < n_iterations; i++) {
    foo0();
  }
  result = gettimeofday(&end_time, NULL);
  return elapsed(start_time, end_time) / 1000.0;
}

double measure_foo1(int n_iterations) {
  int result;

  unsigned long long int x;
  unsigned long long int y;
  double sum = 0;
  unsigned int lo,hi;

  for (int i = 0; i < n_iterations; i++) {
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    x = ((long long)hi << 32) | lo;
    foo1(1);
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    y = ((long long)hi << 32) | lo;
    sum += y - x;
  }
  return sum;
}

double measure_foo2(int n_iterations) {
  int result;

  unsigned long long int x;
  unsigned long long int y;
  double sum = 0;
  unsigned int lo,hi;

  for (int i = 0; i < n_iterations; i++) {
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    x = ((long long)hi << 32) | lo;
    foo2(1, 1);
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    y = ((long long)hi << 32) | lo;
    sum += y - x;
  }
  return sum;
}

double measure_foo3(int n_iterations) {
  int result;

  unsigned long long int x;
  unsigned long long int y;
  double sum = 0;
  unsigned int lo,hi;

  for (int i = 0; i < n_iterations; i++) {
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    x = ((long long)hi << 32) | lo;
    foo3(1, 1, 1);
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    y = ((long long)hi << 32) | lo;
    sum += y - x;
  }
  return sum;
}

double measure_foo4(int n_iterations) {
  int result;

  unsigned long long int x;
  unsigned long long int y;
  double sum = 0;
  unsigned int lo,hi;

  for (int i = 0; i < n_iterations; i++) {
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    x = ((long long)hi << 32) | lo;
    foo4(1, 1, 1, 1);
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    y = ((long long)hi << 32) | lo;
    sum += y - x;
  }
  return sum;
}

double measure_foo5(int n_iterations) {
  int result;

  unsigned long long int x;
  unsigned long long int y;
  double sum = 0;
  unsigned int lo,hi;

  for (int i = 0; i < n_iterations; i++) {
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    x = ((long long)hi << 32) | lo;
    foo5(1, 1, 1, 1, 1);
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    y = ((long long)hi << 32) | lo;
    sum += y - x;
  }
  return sum;
}

double measure_foo6(int n_iterations) {
  int result;

  unsigned long long int x;
  unsigned long long int y;
  double sum = 0;
  unsigned int lo,hi;

  for (int i = 0; i < n_iterations; i++) {
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    x = ((long long)hi << 32) | lo;
    foo6(1, 1, 1, 1, 1, 1);
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    y = ((long long)hi << 32) | lo;
    sum += y - x;
  }
  return sum;
}

double measure_foo7(int n_iterations) {
  int result;

  unsigned long long int x;
  unsigned long long int y;
  double sum = 0;
  unsigned int lo,hi;

  for (int i = 0; i < n_iterations; i++) {
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    x = ((long long)hi << 32) | lo;
    foo7(1, 1, 1, 1, 1, 1, 1);
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    y = ((long long)hi << 32) | lo;
    sum += y - x;
  }
  return sum;
}

int main(){
  initialize();
  unsigned int NUM_TRIALS = 100;
  unsigned int NUM_ITERATIONS = 1000000;
  printf("Number of trials: %u\n", NUM_TRIALS);
  printf("Iterations per trial: %u\n", NUM_ITERATIONS);

  double foo0_time = 0.0;
  double foo1_time = 0.0;
  double foo2_time = 0.0;
  double foo3_time = 0.0;
  double foo4_time = 0.0;
  double foo5_time = 0.0;
  double foo6_time = 0.0;
  double foo7_time = 0.0;

  for (int i = 0; i < NUM_TRIALS; i++) {
    foo0_time += measure_foo0(NUM_ITERATIONS);
    foo1_time += measure_foo1(NUM_ITERATIONS);
    foo2_time += measure_foo2(NUM_ITERATIONS);
    foo3_time += measure_foo3(NUM_ITERATIONS);
    foo4_time += measure_foo4(NUM_ITERATIONS);
    foo5_time += measure_foo5(NUM_ITERATIONS);
    foo6_time += measure_foo6(NUM_ITERATIONS);
    foo7_time += measure_foo7(NUM_ITERATIONS);
  }
  printf("Elapsed time for foo0(): %f ms\n", foo0_time / NUM_TRIALS);
  printf("Elapsed time for foo1(): %f ms\n", foo1_time / NUM_TRIALS);
  printf("Elapsed time for foo2(): %f ms\n", foo2_time / NUM_TRIALS);
  printf("Elapsed time for foo3(): %f ms\n", foo3_time / NUM_TRIALS);
  printf("Elapsed time for foo4(): %f ms\n", foo4_time / NUM_TRIALS);
  printf("Elapsed time for foo5(): %f ms\n", foo5_time / NUM_TRIALS);
  printf("Elapsed time for foo6(): %f ms\n", foo6_time / NUM_TRIALS);
  printf("Elapsed time for foo7(): %f ms\n", foo7_time / NUM_TRIALS);
}

