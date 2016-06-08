#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include <netdb.h>

#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <util.h>
#include <init.h>

#define BUFF_SIZE 1024

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
    printf("\n Usage: %s hostname port count size\n", argv[0]);
    return 1;
  }


  char ip_address[100];
  get_ip(argv[1], ip_address);
  fprintf(stderr, "%s resolved to : %s\n" , argv[1], ip_address);


  int sockfd = 0;
  sockfd = init_socket(ip_address, atoi(argv[2]));

  int n;
  char recvBuff[BUFF_SIZE];
  char message[atoi(argv[4]) + 1];

  memset(recvBuff, '\0', sizeof(recvBuff));
  memset(message, '\0', sizeof(message));
  memset(message, '1', sizeof(message) - 1);

  /*char* message = "hi";*/
  char* close_message = "close\n";

  unsigned int count = atoi(argv[3]);

  fprintf(stderr, "Sending %u messages\n", count);

  int result;
  struct timeval start_time;
  struct timeval end_time;

  double data[count];

  for (int i = 0; i < count; i++) {
     result = gettimeofday(&start_time, NULL);
     n = write(sockfd, message, strlen(message));
     if (n < 0) {
        error("Error: writing to socket");
     }
     n = read(sockfd, recvBuff, BUFF_SIZE);
     result = gettimeofday(&end_time, NULL);
     data[i] = elapsed(start_time, end_time) / 1000.0;
     printf("%f\n", data[i]);
  }
  write(sockfd, close_message, strlen(close_message));
  read(sockfd, recvBuff, BUFF_SIZE);

  fprintf(stderr, "Average RTT per packet: %f ms\n", mean(data, count));
  fprintf(stderr, "Stdev RTT per packet: %f ms\n", stdev(data, count));

  return 0;
}
