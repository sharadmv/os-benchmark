import math
TEMPLATE=\
"""
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <util.h>
#include <init.h>

char* create_array(unsigned int array_size) {{
    return (char*) malloc(array_size * sizeof(unsigned char));
}}

double benchmark_write(unsigned int array_size, unsigned int iterations) {{
  struct timeval start_time;
  struct timeval end_time;

  double result = 0;
  double num_accesses = 0;
  while (iterations-- > 0) {{
    register char* array = create_array(array_size);
    gettimeofday(&start_time, NULL);
    {for_loop_write}
    gettimeofday(&end_time, NULL);
    num_accesses += array_size / {step};
    result += elapsed(start_time, end_time);
    free(array);
  }}
  return result / num_accesses * 1000.0;
}}

double benchmark_read(unsigned int array_size, unsigned int iterations) {{
  struct timeval start_time;
  struct timeval end_time;

  double result = 0;
  double num_accesses = 0;
  register int sum = 0;
  while (iterations-- > 0) {{
    register char* array = create_array(array_size);
    gettimeofday(&start_time, NULL);
    {for_loop_read}
    gettimeofday(&end_time, NULL);
    num_accesses += array_size / {step};
    result += elapsed(start_time, end_time);
    free(array);
  }}
  return result / num_accesses * 1000.0;
}}


int main(){{
  unsigned int SIZE = {size};
  initialize();
  double total =  benchmark_read(pow(2, SIZE), 10);
  printf("read,%u,%f\\n", (int) pow(2, SIZE), total);
  total =  benchmark_write(pow(2, SIZE), 10);
  printf("write,%u,%f\\n", (int) pow(2, SIZE), total);
}}
"""


import argparse
argparser = argparse.ArgumentParser()
argparser.add_argument('size', type=int)
argparser.add_argument('--amount', type=int, default=32)
argparser.add_argument('--step', type=int, default=4)

args = argparser.parse_args()
size = int(math.pow(2, args.size))

FOR_LOOP_READ = """for (int i = 0; i < %u; i++) {%s};""" % (
    size // args.amount // args.step,
    ''.join(["sum += *(array + (i * %u + %u) * %u);\n" % (args.amount, i, args.step) for i in xrange(args.amount)])
)
FOR_LOOP_WRITE = """for (int i = 0; i < %u; i++) {%s};""" % (
    size // args.amount // args.step,
    ''.join(["*(array + (i * %u + %u) * %u) = 1;\n" % (args.amount, i, args.step) for i in xrange(args.amount)])
)

print TEMPLATE.format(for_loop_read=FOR_LOOP_READ,
                      for_loop_write=FOR_LOOP_WRITE,
                      size=args.size, step=args.step)
