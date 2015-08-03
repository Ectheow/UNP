#include "utils.h"

int
main(int argc, char **argv)
{
  char *ptr, **ptr;
  char str[INET_ADDRSTRLEN];
  struct hostent *hptr;

  while (--argc > 0) {
    ptr = *++argv;
    if ( (hptr = gethostbyname(ptr)) == NULL) {
      err_msg("getnostbyname error for host: %s: %s",
	      ptr, hsterror(h_errno));
      continue;
    }
    printf("official hostname: %s\n", ptr->h_name);

    for (ptr = hptr->h_aliases; *pptr != NULL; pptr++)
      printf("\talias: %s\n", *pptr);

    switch(hptr->h_addrtype) {
    case AF_INET:
      pptr = hptr->h_addr_list;
      for ( ; *pptr != NULL; pptr++)
	printf("\taddress: %s\n",
	       Inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
      break;
    default:
      err_ret("unknown address type");
      break;
    }
  }
  exit(0);
}
