#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <linux/ip.h>
#include <linux/icmp.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#include <util.h>
#include <init.h>

typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned long u_long;
#define h_addr h_addr_list[0]

unsigned short in_cksum(unsigned short *addr, int len)
{
  // Taken from: http://stackoverflow.com/questions/13620607/creating-ip-network-packets
  register int sum = 0;
  u_short answer = 0;
  register u_short *w = addr;
  register int nleft = len;
  while (nleft > 1)
  {
    sum += *w++;
    nleft -= 2;
  }
  if (nleft == 1)
  {
    *(u_char *) (&answer) = *(u_char *) w;
    sum += answer;
  }
  sum = (sum >> 16) + (sum & 0xffff);
  sum += (sum >> 16);
  answer = ~sum;
  return answer;
}

int main(int argc, char* argv[])
{
  struct iphdr* ip;
  struct iphdr* reply;
  struct icmphdr* icmp;
  struct sockaddr_in connection;
  char* packet;
  char* buffer;
  int sockfd;
  int optval;
  int addrlen;
  int siz;
  unsigned int count;

  char dst_addr[20];
  char src_addr[20];

  get_ip(argv[1], dst_addr);
  get_ip(argv[2], src_addr);
  count = atoi(argv[3]);

  fprintf(stderr, "Count: %u\n", count);
  fprintf(stderr, "Source address: %s\n", src_addr);
  fprintf(stderr, "Destination address: %s\n", dst_addr);

  packet = malloc(sizeof(struct iphdr) + sizeof(struct icmphdr));
  buffer = malloc(sizeof(struct iphdr) + sizeof(struct icmphdr));
  ip = (struct iphdr*) packet;
  icmp = (struct icmphdr*) (packet + sizeof(struct iphdr));
  ip->ihl = 5;
  ip->version = 4;
  ip->tos = 0;
  ip->tot_len = sizeof(struct iphdr) + sizeof(struct icmphdr);
  ip->id = htons(0);
  ip->frag_off = 0;
  ip->ttl = 64;
  ip->protocol = IPPROTO_ICMP;
  ip->saddr = inet_addr(src_addr);
  ip->daddr = inet_addr(dst_addr);
  ip->check = in_cksum((unsigned short *)ip, sizeof(struct iphdr));
  sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
  setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &optval, sizeof(int));
  icmp->type = ICMP_ECHO;
  icmp->code = 0;
  icmp->un.echo.id = random();
  icmp->un.echo.sequence = 0;
  icmp-> checksum = in_cksum((unsigned short *)icmp, sizeof(struct icmphdr));
  connection.sin_family = AF_INET;
  connection.sin_addr.s_addr = inet_addr(dst_addr);

  int result;
  struct timeval start_time;
  struct timeval end_time;
  double data[count];
  for (int i = 0; i < count; i++) {
    result = gettimeofday(&start_time, NULL);
    sendto(sockfd, packet, ip->tot_len, 0, (struct sockaddr *)&connection, sizeof(struct sockaddr));
    addrlen = sizeof(connection);
    recvfrom(sockfd, buffer, sizeof(struct iphdr) + sizeof(struct icmphdr), 0, (struct sockaddr *)&connection, &addrlen);
    reply = (struct iphdr*) buffer;
    result = gettimeofday(&end_time, NULL);
    data[i] = elapsed(start_time, end_time) / 1000.0;
    printf("%f\n", data[i]);
  }
  fprintf(stderr, "Average RTT per packet: %f ms\n", mean(data, count));
  fprintf(stderr, "Stdev RTT per packet: %f ms\n", stdev(data, count));

  free(packet);
  free(buffer);
  close(sockfd);
  return 0;
}

