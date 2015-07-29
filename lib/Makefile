include ../Make.defines
LIB_SRCS= $(wildcard *.c)
LIB_OBJS= $(patsubst %.c,%,$(LIB_SRCS))

all: $(LIB_OBJS)
	ar rv $(LIBUNP) $? 
	$(RANLIB) $(LIBUNP)
clean:
	$(RM) $(LIB_OBJS) $(LIBUNP_NAME)
