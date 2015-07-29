include Makefile.inc
ifeq ($(DEBUG),y)
	DEBFLAGS= -g -DDEBUG
else
	DEBFLAGS=
endif

CCFLAGS+=$(DEBFLAGS)
#order is important in this case. lib needs to be first.
DIRS = lib echo udpcliserv

all: force_do
	for i in $(DIRS); do \
		(cd $$i; $(MAKE) $(MFLAGS));\
	done

clean:
	for i in $(DIRS); do (cd $$i; $(MAKE) clean); done
force_do:
	true

