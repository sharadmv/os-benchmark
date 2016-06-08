#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>

#include <util.h>
#include <init.h>

double benchmark(unsigned int n_iterations) {
  unsigned char *f;
  int sum;
  struct timeval start_time;
  struct timeval end_time;

  double result1 = 0;
  double result2 = 0;
  for (int i = 0; i < n_iterations; i++) {
    long pagesize = sysconf(_SC_PAGESIZE);
    unsigned char *p = malloc(pagesize + 1); /* Cross page boundaries. Page fault may occur depending on your allocator / libc implementation. */
    gettimeofday(&start_time, NULL);
    p[0] = 0;        /* Page fault. */
    gettimeofday(&end_time, NULL);
    result1 += elapsed(start_time, end_time);
    gettimeofday(&start_time, NULL);
    p[pagesize] = 1; /* Page fault. */
    gettimeofday(&end_time, NULL);
    result2 += elapsed(start_time, end_time);
  }
  printf("1: %f\n", result1);
  printf("2: %f\n", result2);
}
int main()
{
    initialize();
    long pagesize = sysconf(_SC_PAGESIZE);
    printf("%lu\n", pagesize);
    benchmark(10000);
    return 0;
}
