#include "utils.h"

void
dg_echo(int sockfd, struct sockaddr *pcliaddr, socklen_t clilen)
{
  char buf[MAXLINE];
  char cli[INET_ADDRSTRLEN];
  socklen_t len;
  ssize_t n;
  
  for ( ; ; ) {
    len = clilen;
    n = Recvfrom(sockfd, buf, MAXLINE, 0, pcliaddr, &len);
    printf("Recieved client : %s:%d\n",
	   inet_ntop(AF_INET,
		     &((struct sockaddr_in *)pcliaddr)->sin_addr.s_addr,
		     cli, clilen),
	   ntohs(((struct sockaddr_in *)pcliaddr)->sin_port));
    sleep(5);
    Sendto(sockfd, buf, n, 0, pcliaddr, len);
  }
}

int
main(int argc, char **argv)
{
  int sockfd;
  struct sockaddr_in servaddr, cliaddr;

  sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(SERV_PORT);

  Bind(sockfd, (SA *)&servaddr, sizeof(servaddr));
  dg_echo(sockfd, (SA *)&cliaddr, sizeof(cliaddr));
}

