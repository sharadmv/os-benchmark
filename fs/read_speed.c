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
  if (argc < 3) {
    printf("Usage: %s count file1 file2...\n", argv[0]);
    exit(1);
  }

  int count = atoi(argv[1]);

  int result;
  struct timeval start_time;
  struct timeval end_time;

  double sequential_data[count];
  double random_data[count];

  unsigned int num_blocks;
  unsigned int block_id;

  char buffer[BLOCKSIZE];
  unsigned int numbytes, idx;

  for (int j = 2; j < argc; j++) {

    FILE *f = fopen(argv[j], "rb");
    fseek(f, 0, SEEK_END);
    numbytes = ftell(f);
    fclose(f);

    int fp = open(argv[j], O_RDONLY);
    fprintf(stderr, "Filename: %s\n", argv[j]);

    system("echo 3 > /proc/sys/vm/drop_caches");

    if (fp > 0) {
      num_blocks = (unsigned int) numbytes / BLOCKSIZE;

      // Sequential
      lseek(fp, 0, SEEK_SET);
      result = gettimeofday(&start_time, NULL);
      for (int i = 0; i < count; i++) {
        idx = i % num_blocks;
        /*printf("%u\n", idx);*/
        pread(fp, buffer, BLOCKSIZE, idx * BLOCKSIZE);
      }
      result = gettimeofday(&end_time, NULL);
      fprintf(stderr, "Average sequential read time: %f ms\n", elapsed(start_time, end_time) / 1000.0 / count);

      // Random
      lseek(fp, 0, SEEK_SET);
      result = gettimeofday(&start_time, NULL);
      for (int i = 0; i < count; i++) {
        idx = rand() % num_blocks;
        /*printf("%u\n", idx);*/
        pread(fp, buffer, BLOCKSIZE, idx * BLOCKSIZE);
      }
      result = gettimeofday(&end_time, NULL);
      fprintf(stderr, "Average random read time: %f ms\n", elapsed(start_time, end_time) / 1000.0 / count);
    }
    close(fp);
  }
}
