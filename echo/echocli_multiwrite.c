#include "utils.h"


void
handle_sigpipe(int signal_no)
{
  return;
}
void
str_cli1(FILE *fp, int sockfd)
{
  char sendline[MAXLINE], recvline[MAXLINE];

  while(Fgets(sendline, MAXLINE, fp) != NULL) {
    Writen(sockfd, sendline, 1); /* Read a single byte. */
    /* If the server host program crashes, it sends a fin to us.
     * This could be interpreted as a half-close, meaning that
     * the server simply won't be _sending_ any more data,
     * according to TCP. So technically, writing to the socket
     * _could_ be expected behaviour. And so the kernel doesn't
     * signal us or anything, and lets us write 
     * to the socket. but when we _do_ write,
     * the server host has no open port for us and sends a 
     * reset (RST) message after our first write.
     * When we try to write _again_ to a TCP socket that has
     * recieved a RST, we get a sigpipe, since the kernel
     * knows we aren't supposed to now. The default
     * action is to kill the program. However, if we write,
     * get a RST, and then read (like in clisimple), 
     * the kernel _knows_ that we aren't supposed to 
     * read, since the server sent a FIN.
     * In this case, we get a 0 unexpectedly (for us), and terminate,
     * before trying to write again and causing a sigpipe.
     * In our case the 0 is unexpected because _we_
     * are supposed to terminate in this program with ctrl-D
     * (EOF) on our side and initiate the close. Normally,
     * when we get an EOF (fgets == NULL), then we return,
     * after which time the socket is closed w/ a syscall,
     * which initiates the TCP close procedure, in the 
     * kernel's TCP/IP stack.
     *
     * One problem here is we hang on the fgets until the user
     * input finishes, when in fact while they are typing
     * the server could have crashed. This results in odd behaviour
     * when we try to write. 
     */
    sleep(1);
    Writen(sockfd, sendline+1, strlen(sendline)-1);

    if (Readline(sockfd, recvline, MAXLINE) == 0)
      err_quit("str_cli: server terminated prematurely.");

    Fputs(recvline, stdout);
  }
    
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

  
