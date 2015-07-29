#include "utils.h"

int
main(int argc, char **argv)
{
  int sockfd;
  char sendline[MAXLINE];
  struct sockaddr_in to_addr;

  if(argc != 3)
    err_quit("usage: %s <IPaddress> <port>", argv[0]);

  bzero(&to_addr, sizeof(to_addr));
  to_addr.sin_family = AF_INET;
  Inet_pton(AF_INET, argv[1], &to_addr.sin_addr);
  to_addr.sin_port = htons(atoi(argv[2]));
  printf("portno: %d\n", atoi(argv[2]));
  sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
  
  printf("type a line > ");
  Fgets(sendline, MAXLINE, stdin);
  
  Sendto(sockfd, sendline, strlen(sendline), 0, (SA *)&to_addr, sizeof(to_addr));

  exit(0);
}
