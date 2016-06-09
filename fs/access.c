#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>

#include <sys/time.h>
#include <unistd.h>
#include <util.h>

#define MAXBUFLEN 1000000
#define BLOCKSIZE 4096

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("Usage: %s filename\n", argv[0]);
    exit(1);
  }

  char* filename = argv[1];

  unsigned int num_blocks;

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
    while(1) {
      idx = idx % num_blocks;
      pread(fp, buffer, BLOCKSIZE, idx * BLOCKSIZE);
      idx += 1;
    }
  }
  close(fp);
}
