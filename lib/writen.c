#include "utils.h"

ssize_t
writen(int fd, const void *vptr, size_t n)
{
  size_t nleft;
  ssize_t nwritten;
  const char *ptr;

  ptr = vptr;
  nleft = n;
  while(nleft > 0) {
    if ( (nwritten = write(fd, ptr, nleft)) <= 0) {
      if(nwritten < 0 && errno == EINTR)
	nwritten = 0;		/* Try again. */
      else
	return(-1);		/* Unhandled error */
    }

    nleft -= nwritten;
    ptr += nwritten;
  }
  return (n);
}


/* Don't keep track of return value because
 * if it works as expected, we already
 * know it.
 */
void
Writen(int fd, const void *vptr, size_t n)
{
  if (writen(fd, vptr, n) != n)
    err_sys("writen error");
}

