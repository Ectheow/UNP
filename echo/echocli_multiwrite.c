#include "utils.h"


void
handle_sigpipe(int signal_no)
{
  return;
}

int
main(int argc, char **argv)
{
  int sockfd;
  struct sockaddr_in servaddr;

  if(argc != 2)
    err_quit("usage: tcpcli <IPaddress>");

  sockfd = Socket(AF_INET, SOCK_STREAM, 0);

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(SERV_PORT);
  Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
  Signal(SIGPIPE, handle_sigpipe);
  Connect(sockfd, (SA *)&servaddr, sizeof(servaddr));

  /* str_cli1 writes twice to a sockfd.
   * This is an attempt to cause a 
   * SIGPIPE signal when the server terminates.
   * Because we get an RST after the first
   * and attempt another write, this
   * elicits a SIGPIPE signal.
   */
  str_cli1(stdin, sockfd);

  exit(0);
}

  
