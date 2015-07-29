ifeq ($(DEBUG),y)
	DEBFLAGS= -g -DDEBUG
else
	DEBFLAGS=
endif

CC=gcc
CCFLAGS=$(DEBFLAGS)
SRCS = $(wildcard *.c)
PROGS = $(patsubst %.c,%,$(SRCS))
INCLUDES=../include
LIBS=../lib
all: $(PROGS)

%: %.c
	$(CC) -L$(LIBS) -I$(INCLUDES) $(CCFLAGS) -o $@ $< -lunp
clean:
	rm -rf $(PROGS)
