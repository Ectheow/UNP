#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdarg.h>
#include <signal.h>

#ifndef _UTILS_H_
#define _UTILS_H_

#define LISTENQ 1024
#define MAXLINE 4096
#define SERV_PORT 9877

#define SA struct sockaddr

typedef void Sigfunc(int);

void err_sys(const char *fmt, ...);
void err_quit(const char *fmt, ...);


/*-- Wrappers --*/
static void
Close(int fd_to_close)
{
  if(close(fd_to_close) == -1)
    err_sys("close error");
}

static int
Accept(int fd, struct sockaddr *sa, socklen_t *salenptr)
{
  int n;
 again:
  if( (n = accept(fd, sa, salenptr)) < 0) {
    if(errno == ECONNABORTED)
      goto again;
    else
      err_sys("accept error");
  }
  return(n);
}


static void
Bind(int fd, const struct sockaddr *sa, socklen_t salen)
{
  if(bind(fd, sa, salen) < 0)
    err_sys("bind error");
}

static void
Listen(int fd, int backlog)
{
  char *ptr;

  if( (ptr = getenv("LISTENQ")) != NULL)
    backlog = atoi(ptr);
  if(listen(fd, backlog) < 0)
    err_sys("listen error");
}

static void
Write(int fd, void *buff, size_t nbytes)
{
  if(write(fd, buff, nbytes) != nbytes)
    err_sys("write error");
}


static int
Socket(int family, int type, int protocol)
{
  int n;
  if( (n = socket(family, type, protocol)) < 0)
    err_sys("socket error");
  return(n);
}

static ssize_t
Read(int fd, void *ptr, size_t nbytes)
{
  ssize_t n;

  if( (n = read(fd, ptr, nbytes)) == -1)
    err_sys("read error");
  return(n);
}


static void
Connect(int fd, const struct sockaddr *sa, socklen_t salen)
{
  if(connect(fd, sa, salen) < 0)
    err_sys("connect error");
}

static pid_t
Fork(void)
{
  pid_t pid;

  if( (pid = fork()) == -1)
    err_sys("fork error");
  return(pid);
}


static void
Inet_pton(int family, const char *strptr, void *addrptr)
{
  int n;

  if ( (n = inet_pton(family, strptr, addrptr)) < 0)
    err_sys("inet_pton error for %s", strptr);
  else if(n == 0)
    err_quit("inet_pton error for %s", strptr);
}

static char *
Fgets(char *ptr, int n, FILE *stream)
{
  char *rptr;

  if ( (rptr = fgets(ptr, n, stream)) == NULL && ferror(stream))
    err_sys("fgets error");

  return (rptr);
}

static void
Fputs(const char *ptr, FILE *stream)
{
  if(fputs(ptr, stream) == EOF)
    err_sys("fputs error");
}

static int
Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
       struct timeval *timeout)
{
  int n;

  if ( (n = select(nfds, readfds, writefds, exceptfds, timeout)) < 0)
    err_sys("select error");
  return (n);			/* May return 0 on timeout. */
  
}
#endif	/* _UTILS_H_ */


