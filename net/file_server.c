#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE 1024 * 1024

void error(char *msg) {
  printf(msg);
  exit(1);
}

int main(int argc, char **argv) {
  int listenfd;
  int connfd;
  int portno;
  int clientlen;
  struct sockaddr_in serveraddr;
  struct sockaddr_in clientaddr;
  struct hostent *hostp;
  char message[BUFSIZE];
  int optval;
  int n;

  if (argc != 2) {
    fprintf(stderr, "usage: %s port\n", argv[0]);
    exit(1);
  }
  portno = atoi(argv[1]);

  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  if (listenfd < 0)
    error("ERROR opening socket");

  optval = 1;
  setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,
      (const void *)&optval , sizeof(int));

  bzero((char *) &serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons((unsigned short)portno);

  if (bind(listenfd, (struct sockaddr *) &serveraddr,
        sizeof(serveraddr)) < 0)
    error("ERROR on binding");

  if (listen(listenfd, 5) < 0)
    error("ERROR on listen");

  clientlen = sizeof(clientaddr);

  char buffer[BUFSIZE];
  unsigned int file_size, remain_data;
  int len;
  char* response = "done";

  while (1) {
    connfd = accept(listenfd, (struct sockaddr *) &clientaddr, &clientlen);
    if (connfd < 0)
      error("ERROR on accept");
    recv(connfd, buffer, BUFSIZE, 0);
    file_size = atoi(buffer);
    printf("File size : %u\n", file_size);
    send(connfd, response, strlen(response), 0);

    remain_data = file_size;
    while ((remain_data > 0) && ((len = recv(connfd, buffer, BUFSIZE, 0)) > 0))
    {
      remain_data -= len;
      printf("Received %d bytes and expect: %u bytes\n", len, remain_data);
    }
    send(connfd, response, strlen(response), 0);
    close(connfd);
  }
}
