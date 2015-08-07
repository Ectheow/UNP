#include "utils.h"
int daemon_proc;		/* Set to 1 if we are a daemon. */

static void
err_base(int type, int level, const char *fmt, va_list args) {
  char buf[MAXLINE];
  vsnprintf(buf, MAXLINE, fmt, args);
  va_end(args);
  if(type == 1) {
    strcat(buf, ": ");
    strcat(buf, strerror(errno));
  }
  strcat(buf, "\n");
  if(daemon_proc) {
    syslog(level, buf);
  } else {
    fflush(stdout);
    fputs(buf, stderr);
    fflush(stderr);
  }

  return;
}

/*
 * Fata error message
 * with errno set
 */
void
err_sys(const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  err_base(1, LOG_ERR, fmt, args);
  va_end(args);
  exit(1);
}

/*
 * Fatal error message
 * with errno not set
 */
void
err_quit(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  err_base(0, LOG_ERR, fmt, args);
  va_end(args);
  exit(1);
}

/*
 * Non-fatal error message
 * with errno not set.
 */
void
err_msg(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  err_base(0, LOG_INFO, fmt, args);
  va_end(args);
  return; /* No exit. Not fatal.*/
}

/*
 * Non-fatal error message
 * with errno set
 */
void
err_ret(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  err_base(1, LOG_INFO, fmt, args);
  va_end(args);
  return;
}
