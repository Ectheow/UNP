#include "utils.h"

#define CLI_OPENMAX 12
#define finish_client(client) {			\
    Close(client.fd);				\
    client.fd = -1;				\
    --num_open_clients;				\
  }						\



void
randomize_charbuf(char *buf, size_t size)
{
  size_t i;
  for (i=0; i < size; i+=sizeof(int)) {
    *((int *)(buf+i)) = rand();
  }
}

int
main(int argc, char **argv)
{
  struct pollfd clients[CLI_OPENMAX];
  long num_transmitted[CLI_OPENMAX],
    num_recieved[CLI_OPENMAX];
  struct sockaddr_in serv_addr;
  socklen_t serv_addr_len;
  int i, j, nready, tmp_fd, n;
  char buf[MAXLINE];
  size_t num_bytes, trans_size, num_open_clients;

  if (argc != 3) {
    err_quit("Usage: %s <ip_addr> <num_bytes>", argv[0]);
  }

  num_bytes = atoi(argv[2]);
  Inet_pton(AF_INET, argv[1], &serv_addr.sin_addr.s_addr);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(SERV_PORT);
  
  for (i = 0; i<CLI_OPENMAX; ++i) {
    tmp_fd = Socket(AF_INET, SOCK_STREAM, 0);
    if (connect(tmp_fd, (SA *)&serv_addr, sizeof(serv_addr)) != 0) {
      err_sys("%s: error connecting client #%d to server %s.",
	      argv[0], i, argv[1]);
    }
    clients[i].fd = tmp_fd;
    clients[i].events = POLLOUT | POLLRDNORM;
    num_transmitted[i] = 0;
    num_recieved[i] = 0;
    num_open_clients++;
  }

  while(num_open_clients > 0) {

    nready = Poll(clients, num_open_clients, INFTIM);
    
    for (i=0; i < CLI_OPENMAX; ++i) {
      if(clients[i].fd == -1) {
	continue;
      }
      
      if(clients[i].revents & POLLOUT) {

	trans_size = rand()%MAXLINE;
	randomize_charbuf(buf, trans_size);
	Writen(clients[i].fd, buf, trans_size);

	num_transmitted[i] += trans_size;
	if(num_transmitted[i] >= num_bytes) {
	  Shutdown(clients[i].fd, SHUT_WR);
	}
      } else if (clients[i].revents & (POLLRDNORM | POLLERR) ) {
	/* We know it won't be more than maxline, that's 
	 * how much we send (at most).
	 */
	if ( (n = read(clients[i].fd, MAXLINE) == 0)) {
	  if (errno == ECONNRESET) {
	    finish_client(clients[i]);
	    printf("Reset for client #%d\n", i);
	  }
	} else if (n == 0) {
	  finish_client(clients[i]);
	} else {
	  num_recieved[i] += n;
	}
      }
    }
  }
  return 0;
}
