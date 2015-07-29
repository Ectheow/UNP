include ../Make.defines
SRCS = $(wildcard *.c)
PROGS = $(patsubst %.c,%,$(SRCS))
all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)
clean:
	rm -rf $(PROGS)
