#define _GNU_SOURCE
#include <sched.h>

void initialize() {
  cpu_set_t mask;
  CPU_ZERO(&mask);
  CPU_SET(2, &mask);
  sched_setaffinity(0, sizeof(mask), &mask);
}
