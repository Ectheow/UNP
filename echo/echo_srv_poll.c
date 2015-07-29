#include "utils.h"

#define LISTEN_SOCK_IDX 0

int
main(int argc, char **argv)
{
  int i, maxi, maxfd, listenfd, connfd, sockfd;
  int nready;
  struct pollfd client[OPEN_MAX];
  ssize_t n;
  fd_set rset, allset;
  char buf[MAXLINE];
  char inetaddrbuf[INET_ADDRSTRLEN];
  socklen_t clilen;
  struct sockaddr_in cliaddr, servaddr;

  listenfd = Socket(AF_INET, SOCK_STREAM, 0);

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(SERV_PORT);

  Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));
  Listen(listenfd, LISTENQ);
  
  client[LISTEN_SOCK_IDX].fd = listenfd;
  client[LISTEN_SOCK_IDX].events = POLLRDNORM;
  for (i = 1; i< OPEN_MAX; ++i)
    client[i].fd = -1;
  maxi = 0;			/* Max index */

  for ( ; ; ) {
    nready = Poll(client, maxi+1, INFTIM); /* BLock until a new event happens */

    if (client[LISTEN_SOCK_IDX].revents & POLLRDNORM) {
      clilen = sizeof(cliaddr);
      connfd = Accept(listenfd, (SA *)&cliaddr, &clilen);
      
      inet_ntop(AF_INET, (void *)&(cliaddr.sin_addr.s_addr), inetaddrbuf, clilen);
      printf("Accepted new client from: %s:%d\n", inetaddrbuf, ntohs(cliaddr.sin_port));
      
      /* Put the new client into the clients array. */
      for (i = 1; i< OPEN_MAX; i++)
	if (client[i].fd < 0) {
	  client[i].fd = connfd;
	  break;
	}
      if (i == OPEN_MAX)
	err_quit("too many clients");
      client[i].events = POLLRDNORM;
      if(i > maxi)
	maxi = i;

      if (--nready <= 0)
	continue;
    }

    for (i = 0; i<= maxi; i++) {
      if ( (sockfd = client[i].fd) < 0)
	continue;
      if (client[i].revents & (POLLRDNORM | POLLERR) ) {
	if ( (n = read(sockfd, buf, MAXLINE)) == 0) {
	  if (errno == ECONNRESET) { /* Connection reset by client */
	    Close(sockfd);
	    client[i].fd = -1;
	  }
	} else if (n == 0) {	/* Connection closed by client */
	  Close(sockfd);
	  client[i].fd = -1;
	} else
	  Writen(sockfd, buf, n);
	if(--nready <= 0)
	  break;
      }
    }
  }
}
