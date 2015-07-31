#include "utils.h"

void
str_echo(int sockfd)
{
  ssize_t n;
  char buf[MAXLINE];
 again:
  while ( (n = read(sockfd, buf, MAXLINE)) > 0) {
    //sleep(1); Used to simulate a delayed server,
    //which can result in the client exiting before
    //all daya has been read. 
    Writen(sockfd, buf, n);
  }
  if(n < 0 && errno == EINTR)
    goto again;
  else if (n < 0)
    err_sys("str_echo: read error");
}
