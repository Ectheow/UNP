#include "utils.h"

static void
err_base(int type, const char *fmt, va_list args) {
  char buf[MAXLINE];
  vsnprintf(buf, MAXLINE, fmt, args);
  va_end(args);
  if(type == 1) {
    strcat(buf, ": ");
    strcat(buf, strerror(errno));
    strcat(buf, "\n");
  }
  fputs(buf, stderr);

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
  err_base(1, fmt, args);
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
  err_base(0, fmt, args);
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
  err_base(0, fmt, args);
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
  err_base(1, fmt, args);
  va_end(args);
  return;
}
