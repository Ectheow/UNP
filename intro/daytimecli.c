#include "utils.h"

int
main(int argc, char **argv)
{
  int sockfd, n;
  char recvline[MAXLINE+1];
  struct sockaddr_in servaddr;

  if(argc != 2)
    err_quit("usage: a.out <IPaddress>");

  if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    err_sys("socket error");

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(13);
  if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
    err_quit("inet_pton error for %s", argv[1]);

  Connect(sockfd, (SA *)&servaddr, sizeof(servaddr));

  while( (n = Read(sockfd, recvline, MAXLINE)) > 0) {
    recvline[n] = 0; 		/* null terminate the string. */
    if(fputs(recvline, stdout) == EOF)
      err_sys("fputs error");
  }
  exit(0);
}
