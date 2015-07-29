#include "utils.h"

#define NDG 2000
#define DGLEN 1400
static int count;
static void recvfrom_int(int);

void
dg_echo(int sockfd, SA *pcliaddr, socklen_t clilen)
{
  int n;
  socklen_t len;
  char mesg[MAXLINE];

  Signal(SIGINT, recvfrom_int);

  len = sizeof(n);
  Getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &n, &len);
  if(len == sizeof(n))
    printf("initial SO_RCVBUF size: %d\n", n);
  else
    printf("Error getting SO_RCVBUF size.\n");
  
  n = 220 * 1024;
  /* Set the socket recvbuf size. By default*/
  /* it's  87380. */
  Setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &n, sizeof(n));
  printf("Set new SO_RCVBUF: %d\n", n);
  for ( ; ; ) {
    len = clilen;
    Recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &len);

    count++;
  }
}

static void
recvfrom_int(int signo)
{
  printf("\nrecieved %d datagrams\n", count);
  exit(0);
}

int
main(int argc, char **argv)
{
  int sockfd, stream_sockfd;
  struct sockaddr_in cliaddr;
  struct sockaddr_in servaddr;

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(SERV_PORT);
  
  sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
  Bind(sockfd, (SA *)&servaddr, sizeof(servaddr));

  dg_echo(sockfd, (SA *)&cliaddr, sizeof(cliaddr));
}

