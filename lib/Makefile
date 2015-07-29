ifeq ($(DEBUG),y)
	DEBFLAGS=-g -DDEBUG
else
	DEBFLAGS=
endif
CC=gcc
CCFLAGS=$(DEBFLAGS)
INCLUDES=../include
SRCS=$(wildcard *.c)
LIB_OBJS=$(patsubst %.c,%.o,$(SRCS))
LIB_NAME=libunp.a

all: $(LIB_OBJS)
	ar rv $(LIB_NAME) $?
	ranlib $(LIB_NAME)

%.o: %.c
	$(CC) -c $(CCFLAGS) -I$(INCLUDES) -o $@ $<
clean:
	rm -rf $(LIB_OBJS) $(LIB_NAME)
