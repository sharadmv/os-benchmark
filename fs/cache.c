#include<stdio.h>
#include<stdlib.h>

#include <sys/time.h>
#include <unistd.h>
#include<util.h>

#define MAXBUFLEN 1000000

int main(int argc, char** argv) {
  if (argc < 3) {
    printf("Usage: %s count file1 file2 ...\nl", argv[0]);
    exit(1);
  }

  int count = atoi(argv[1]);

  int result;
  struct timeval start_time;
  struct timeval end_time;

  double data[count];

  char* buffer;
  long numbytes;

  for (int j = 2; j < argc; j++) {
    char* filename = argv[j];
    FILE *fp = fopen(filename, "r");
    if (fp != NULL) {
      for (int i = 0; i < count; i++) {
        fseek(fp, 0L, SEEK_SET);
        fseek(fp, 0L, SEEK_END);
        numbytes = ftell(fp);
        fseek(fp, 0L, SEEK_SET);

        buffer = (char*)calloc(numbytes, sizeof(char));

        if(buffer == NULL)
          return 1;


        result = gettimeofday(&start_time, NULL);
        fread(buffer, sizeof(char), numbytes, fp);
        result = gettimeofday(&end_time, NULL);
        data[i] = elapsed(start_time, end_time) / 1000.0;
        free(buffer);
      }
    }
    fclose(fp);
    fprintf(stderr, "File: %s\n", filename);
    fprintf(stderr, "Average read time: %f ms\n", mean(data, count));
    fprintf(stderr, "Stdev read time: %f ms\n", stdev(data, count));
    fprintf(stderr, "\n", filename);
  }
}
