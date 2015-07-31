#include "utils.h"


void
sig_chld(int signo)
{
  pid_t pid;
  int stat;
  while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0)
    printf("child %d terminated\n", pid);
  return;
}

int main(int argc, char **argv)
{
  int listenfd, connfd;
  pid_t childpid;
  socklen_t clilen;
  struct sockaddr_in cliaddr, servaddr;

  listenfd = Socket(AF_INET, SOCK_STREAM, 0);

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(SERV_PORT);

  Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));

  Listen(listenfd, LISTENQ);
  Signal(SIGCHLD, sig_chld);
  
  for( ; ; ) {
    clilen = sizeof(cliaddr);
    /* Restart the system call if we get EINTR.
     * This happens if you don't set the SA_RESTART
     * flag in your sa_flags for the sigaction in 
     * a signal handler; accept can get interrupted.
     * On return, and errno is set to EINTR.
     * In general the reception of signals during a slow
     * system call can result in the restart of a system
     * call when the process recieves an interrupt.
     * a system call is slow if it can block for long periods
     * of time -- network connections or terminals for example.
     */
    if ( (connfd = accept(listenfd, (SA *)&cliaddr, &clilen)) < 0) {
      if(errno == EINTR)
	continue;
      else
	err_sys("accept error");
    }

    if( (childpid = Fork()) == 0) {
      Close(listenfd);		/* This only decrements the refcount, it stays open in parent. */
      str_echo(connfd);
      exit(0);
    }
    Close(connfd);		/* Again, doesn't actually cause a tcp close. */
  }
}  
