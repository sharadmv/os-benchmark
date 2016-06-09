#include <sys/time.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/io.h>
#include <sys/mman.h>
#include <util.h>
#include <init.h>

double benchmark(unsigned int n_iterations) {
  unsigned char *f;
  int sum, sum2;
  struct timeval start_time;
  struct timeval end_time;

  double result = 0, result2;
  int x;
  int* y = &x;
  *y = sum2;
  for (int i = 0; i < n_iterations; i++) {
    const char * file_name = "mem-use.png";
    int fd = open (file_name, O_RDONLY);

    /* Get the size of the file. */
    struct stat s;
    int status = fstat (fd, & s);
    int size = s.st_size;
    f = (char *) mmap (0, size, PROT_READ, MAP_PRIVATE, fd, 0);
    gettimeofday(&start_time, NULL);
    sum += f[0] + f[20000];
    gettimeofday(&end_time, NULL);
    munmap(f, size);
    result += elapsed(start_time, end_time);
    gettimeofday(&start_time, NULL);
    sum2 += *y;
    gettimeofday(&end_time, NULL);
    result2 += elapsed(start_time, end_time);
    close(fd);
  }
  printf("%u\n", sum);
  return result * 1000.0 / n_iterations;
}

int main(int argc, char const *argv[])
{
  initialize();
  printf("%f\n", benchmark(1000000));
}
