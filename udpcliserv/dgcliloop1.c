#include "utils.h"

#define NDG 2000		/* Number Datagrams to send */
#define DGLEN 1400		/* length of each datagram */

/*
 * Just send a whole bunch of empty datagrams.
 */
void
dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
  int i;
  char sendline[DGLEN];

  for(i = 0; i< NDG; ++i) {
    Sendto(sockfd, sendline, DGLEN, 0, pservaddr, servlen);
  }
}

int
main(int argc, char **argv)
{
  int sockfd;
  struct sockaddr_in servaddr;

  if(argc != 2)
    err_quit("usage: %s <IPaddress>", argv[0]);

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
  servaddr.sin_port = htons(SERV_PORT);

  sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

  dg_cli(stdin, sockfd, (SA *)&servaddr, sizeof(servaddr));

  exit(0);
}
