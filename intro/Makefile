CC=gcc
CFLAGS=
SRCS = $(wildcard *.c)
PROGS = $(patsubst %.c,%,$(SRCS))
INCLUDES=../include
LIBS=../lib
all: $(PROGS)

%: %.c
	$(CC) -L$(LIBS) -I$(INCLUDES) $(CFLAGS) -o $@ $< -lunp
clean:
	rm -rf $(PROGS)
