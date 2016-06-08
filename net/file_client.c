#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include <netdb.h>
#include <sys/stat.h>

#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <util.h>
#include <init.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

void error(char* message) {
  perror(message);
  exit(1);
}

int init_socket(char* hostname, int port) {
  int sockfd = 0;
  struct sockaddr_in serv_addr;

  if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    error("Error: Could not create socket\n");
    return 1;
  }

  memset(&serv_addr, '0', sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);

  if(inet_pton(AF_INET, hostname, &serv_addr.sin_addr)<=0)
  {
    error("Error: inet_pton error occured\n");
  }

  if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
  {
    error("Error: connect failed\n");
  }
  return sockfd;
}

int main(int argc , char *argv[])
{

  if(argc != 5)
  {
    printf("\n Usage: %s hostname port file size\n", argv[0]);
    return 1;
  }


  char ip_address[100];
  get_ip(argv[1], ip_address);
  fprintf(stderr, "%s resolved to : %s\n" , argv[1], ip_address);

  int sockfd = 0;
  sockfd = init_socket(ip_address, atoi(argv[2]));

  int n;

  int input_file = open(argv[3], O_RDONLY);
  printf("%s\n", argv[3]);
  unsigned int BUFSIZE = atoi(argv[4]);
  char buffer[BUFSIZE];
  struct stat file_stat;

  fstat(input_file, &file_stat);
  printf("%d\n", file_stat.st_size);

  char file_size[256];
  char response[256];
  sprintf(file_size, "%d", file_stat.st_size);

  int result;
  struct timeval start_time;
  struct timeval end_time;

  struct timeval b_start_time;
  struct timeval b_end_time;

  send(sockfd, file_size, sizeof(file_size), 0);
  recv(sockfd, response, 256, 0);
  printf("Response: %s\n", response);
  result = gettimeofday(&start_time, NULL);
  double max_bandwidth = -1;
  double min_bandwidth = 9999999999999999999999;
  double bandwidth = 0;
  while (1) {
    int bytes_read = read(input_file, buffer, sizeof(buffer));
    /*printf("Read %u bytes\n", bytes_read);*/
    if (bytes_read == 0)
      break;

    if (bytes_read < 0) {
    }

    void *p = buffer;
    while (bytes_read > 0) {
      result = gettimeofday(&b_start_time, NULL);
      int bytes_written = write(sockfd, p, bytes_read);
      result = gettimeofday(&b_end_time, NULL);
      bandwidth = 8 * bytes_written / (elapsed(b_start_time, b_end_time) / 1000.0 / 1000.0);
      if (bandwidth > max_bandwidth) {
        max_bandwidth = bandwidth;
      }
      if (bandwidth < min_bandwidth) {
        min_bandwidth = bandwidth;
      }
      if (bytes_written <= 0) {
      }
      bytes_read -= bytes_written;
      p += bytes_written;
    }
  }
  result = gettimeofday(&end_time, NULL);
  recv(sockfd, response, 256, 0);
  printf("Elapsed: %f\n", elapsed(start_time, end_time) / 1000.0);
  printf("Throughput: %f B/s\n", file_stat.st_size / (elapsed(start_time, end_time) / 1000.0 / 1000.0));
  printf("Max Bandwidth: %f bits/s\n", max_bandwidth);
  printf("Min Bandwidth: %f bits/s\n", min_bandwidth);
  return 0;
}
