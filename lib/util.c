#include<stdio.h>
#include <stdlib.h>
#include<string.h>    //strlen
#include <sys/time.h>
#include <math.h>
#include<arpa/inet.h> //inet_addr
#include <netdb.h>

long elapsed(struct timeval start, struct timeval end) {
    long secs = end.tv_sec - start.tv_sec;
    long usecs = end.tv_usec - start.tv_usec;
    return secs * 1000000 + usecs;
}

double mean(double data[], int n)
{
    double mean=0.0;
    int i;
    for(i=0; i<n;++i)
    {
        mean+=data[i];
    }
    mean=mean/n;
    return mean;
}

double stdev(double data[], int n)
{
    double mean=0.0, sum_deviation=0.0;
    int i;
    for(i=0; i<n;++i)
    {
        mean+=data[i];
    }
    mean=mean/n;
    for(i=0; i<n;++i)
    sum_deviation+=(data[i]-mean)*(data[i]-mean);
    return sqrt(sum_deviation/n);
}

void error(char* message) {
  perror(message);
  exit(1);
}

void get_ip(char* hostname, char* ip) {
  struct hostent* he;
  struct in_addr **addr_list;
  if ( (he = gethostbyname(hostname) ) == NULL)
  {
    error("gethostbyname failed\n");
  }

  //Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
  addr_list = (struct in_addr **) he->h_addr_list;

  for(int i = 0; addr_list[i] != NULL; i++)
  {
    strcpy(ip , inet_ntoa(*addr_list[i]) );
  }
}
