#include "utils.h"

char *
sock_ntop(const struct sockaddr *sa, socklen_t salen)
{
  char portstr[8];
  static char str[128];		/* Note the use of a static char. Unix domain can be 128 long */

  switch(sa->sa_family) {
  case AF_INET: {
    struct sockaddr_in *sin = (struct sockaddr_in *)sa;

    if(inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL)
      return (NULL);
    if(ntohs(sin->sin_port) != 0) {
      snprintf(portstr, sizeof(portstr), ":%d", ntohs(sin->sin_port));
      strcat(str, portstr);
    }
    return(str);
  }
  case AF_INET6: {
    str[0] = '\0';
    return str;
  }
    
  }
}
	       

char *
Sock_ntop(const struct sockaddr *sa, socklen_t salen)
{
  char *ptr;

  if( (ptr = sock_ntop(sa, salen)) == NULL)
    err_sys("sock_ntop error");
  return(ptr);
}
