#include "utils.h"


//TODO: cmdargs for timer
#define BEGIN 1
#define END 100

static short begin, end;
static itimerval interval;
void usage(char* progname) {
  printf("Usage: %s <IPAddress/Hostname>\n", progname);
  printf("\t[--timeout=<seconds[.microseconds]> (default 5s)\n");
  printf("\t[--range=<begin,end> (default 1 100)\n");
}
int parse_cmdlines(int argc, char **argv) {
  for(i = 2; i< argc; ++i) {
    if(strcmp(argv[i], "--timeout=") == 0) {
      if(sscanf("--timeout=%d.%d",
		interval.it_value.tv_sec,
		interval.it_value.tv_usec) != 0) return -1;
    } else if(strcmp(argv[i], "--range=") == 0) {
      if(sscanf("--range=%d,%d",
		begin,
		end) != 0) return -1;
    } else {
      return -1;
    }
  }
  return 0;
}
int
main(int argc, char **argv)
{
  short i;
  int sockfd, n;
  char connstr[MAXLINE];
  char* srvname;
  struct addrinfo  *iter, *res;
  struct servent *servent;
  struct addrinfo hints = {
    .ai_family = AF_INET,
    .ai_socktype = SOCK_STREAM,
  };
  
  struct itimerval five_second_timerval = {
    /* One shot timer has it_interval of 0. */
    .it_interval = {
      .tv_sec = 0,		/* Seconds */
      .tv_usec=0,		/* Microseconds */
    },
    /* One shot timer has a value of 5  secs.*/
    .it_value = {
      .tv_sec = 5,
      .tv_usec = 0,
    },
  };

  
  if(argc < 2) {
    usage(argv[0]);
    exit(1);
  }

  if ( (n = getaddrinfo(argv[1],  NULL, &hints, &res)) != 0) {
    fprintf(stderr, "gai error: %s\n", gai_strerror(n));
  }

  for(iter = res; iter != NULL; iter = iter->ai_next) {
    for(i = BEGIN; i < END; ++i) {
      ((struct sockaddr_in *)iter->ai_addr)->sin_port = htons(i);
      if ( (servent = getservbyport(htons(i), "tcp")) == NULL) {
	srvname = "none";
      } else {
	srvname = servent->s_name;
      }
      
      snprintf(connstr, MAXLINE, "%s:%s (%s)",
	       iter->ai_canonname,
	       srvname,
	       Sock_ntop(iter->ai_addr, iter->ai_addrlen));


      sockfd = Socket(iter->ai_family, iter->ai_socktype, 0);
      if (connect_timeo(sockfd,
			iter->ai_addr,
			iter->ai_addrlen,
			&five_second_timerval) != 0) {
	printf("Couldn't connect to: %s", connstr);
	if(errno == ETIMEDOUT) {
	  printf("timed out.\n");
	} else printf("\n");
      } else {
	printf("Connected to: %s\n", connstr);
      }
    }
  }
  freeaddrinfo(res);
  return (0);
}
