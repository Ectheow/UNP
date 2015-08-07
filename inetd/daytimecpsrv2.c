#include "utils.h"
#include <time.h>

int
main(int argc, char **argv)
{
  int listenfd, connfd;
  socklen_t addrlen, len;
  struct sockaddr *cliaddr;
  char buf[MAXLINE];
  time_t ticks;

  if(argc < 2 || argc > 3)
    err_quit("usage: daytimecpsrv2 [ <host> ] <service or port>");

  daemon_init(argv[0], 0);

  if (argc == 2)
    listenfd = Tcp_listen(NULL, argv[1], &addrlen);
  else
    listenfd = Tcp_listen(argv[1], argv[2], &addrlen);

  cliaddr = Malloc(addrlen);

  for ( ; ; ) {
    len = addrlen;
    connfd = Accept(listenfd, cliaddr, &len);
    err_msg("connection from %s", Sock_ntop(cliaddr, len));

    ticks = time(NULL);
    snprintf(buf, sizeof(buf), "%.24s\r\n", ctime(&ticks));
    Write(connfd, buf, strlen(buf));

    Close(connfd);
  }
}
