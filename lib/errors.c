#include "utils.h"


void
err_sys(const char *fmt, ...)
{
  char buf[MAXLINE];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buf, MAXLINE, fmt, args);
  va_end(args);
  strcat(buf, ": ");
  strcat(buf, strerror(errno));
  strcat(buf, "\n");
  fputs(buf, stderr);
  exit(1);
}

void
err_quit(const char *fmt, ...) {
  char buff[MAXLINE];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buff, MAXLINE, fmt, args);
  va_end(args);
  strcat(buff, "\n");
  fputs(buff, stderr);
  exit(1);
}
