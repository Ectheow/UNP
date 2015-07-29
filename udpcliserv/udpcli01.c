#include "utils.h"

void
dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
  int n;
  char sendline[MAXLINE], recvline[MAXLINE + 1];

  Connect(sockfd, (SA *) pservaddr, servlen);
  
  while (Fgets(sendline, MAXLINE, fp) != NULL) {

    Write(sockfd, sendline, strlen(sendline));
    
    n = Read(sockfd, recvline, MAXLINE);

    recvline[n] = 0;
    Fputs(recvline, stdout);
  }
}

int
main(int argc, char **argv)
{
  int sockfd;
  struct sockaddr_in servaddr;

  if (argc != 2)
    err_quit("usage: %s <IPaddress>", argv[0]);

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
  servaddr.sin_port = htons(SERV_PORT);

  sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

  dg_cli(stdin, sockfd, (SA *)&servaddr, sizeof(servaddr));

  exit(0);
}
