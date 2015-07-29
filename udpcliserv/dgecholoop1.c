#include "utils.h"

static void recvfrom_int(int);
static int count;

void
dg_echo(int sockfd, SA *pcliaddr, socklen_t clilen)
{
  socklen_t len;
  char mesg[MAXLINE];

  Signal(SIGINT, recvfrom_int);

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
  int sockfd;
  struct sockaddr_in cliaddr;
  struct sockaddr_in servaddr;

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(SERV_PORT);
  
  sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
  Bind(sockfd, (SA *)&servaddr, sizeof(servaddr));
  
  dg_echo(sockfd, (SA *)&cliaddr, sizeof(cliaddr));

  exit(0);
  
}
