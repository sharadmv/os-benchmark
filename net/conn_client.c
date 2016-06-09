#include<stdio.h>
#include<stdlib.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include <netdb.h>

#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <util.h>
#include <init.h>

#define BUFF_SIZE 1024 * 1024

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

  if(argc != 4)
  {
    printf("\n Usage: %s hostname port count\n", argv[0]);
    return 1;
  }


  char ip_address[100];
  get_ip(argv[1], ip_address);
  fprintf(stderr, "%s resolved to : %s\n" , argv[1], ip_address);


  int sockfd = 0;
  char* close_message = "close\n";
  unsigned int count = atoi(argv[3]);

  int result;
  struct timeval start_time;
  struct timeval end_time;

  double data[count];
  for (int i = 0; i < count; i++) {
    result = gettimeofday(&start_time, NULL);
    sockfd = init_socket(ip_address, atoi(argv[2]));
    result = gettimeofday(&end_time, NULL);
    data[i] = elapsed(start_time, end_time) / 1000.0;
    printf("%f\n", data[i]);
    write(sockfd, close_message, strlen(close_message));
  }
  fprintf(stderr, "Average connection time: %f ms\n", mean(data, count));
  fprintf(stderr, "Stdev connection time: %f ms\n", stdev(data, count));
  return 0;
}
