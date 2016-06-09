#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>

#include <sys/time.h>
#include <unistd.h>
#include <util.h>

#define MAXBUFLEN 1000000
#define BLOCKSIZE 4096

int main(int argc, char** argv) {
  if (argc != 3) {
    printf("Usage: %s filename count\n", argv[0]);
    exit(1);
  }

  char* filename = argv[1];
  int count = atoi(argv[2]);

  int result;
  struct timeval start_time;
  struct timeval end_time;

  double sequential_data[count];

  unsigned int num_blocks;
  unsigned int block_id;

  char buffer[BLOCKSIZE];
  unsigned int numbytes, idx;

  FILE *f = fopen(filename, "rb");
  fseek(f, 0, SEEK_END);
  numbytes = ftell(f);
  fclose(f);

  int fp = open(filename, O_RDONLY);
  if (fp > 0) {
    num_blocks = (unsigned int) numbytes / BLOCKSIZE;

    lseek(fp, 0, SEEK_SET);
    result = gettimeofday(&start_time, NULL);
    for (int i = 0; i < count; i++) {
      idx = rand() % num_blocks;
      /*printf("%u\n", idx);*/
      pread(fp, buffer, BLOCKSIZE, idx * BLOCKSIZE);
    }
    result = gettimeofday(&end_time, NULL);
    fprintf(stderr, "Average sequential read time: %f ms\n", elapsed(start_time, end_time) / 1000.0 / count);
  }
  close(fp);
}
