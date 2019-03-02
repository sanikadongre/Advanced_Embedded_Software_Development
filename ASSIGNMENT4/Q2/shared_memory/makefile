
CC=gcc

CFLAGS= -O0 -g 

CFILES= shared_memory.c

SRCS= ${CFILES}
OBJS= ${CFILES:.c=.o}

build:	shared_memory

clean:
	-rm -f *.o *.d
	-rm -f shared_memory

pipes:	shared_memory.o
	$(CC)	$(LDFLAGS)	$(CFLAGS)	-o	$@	$@.o	-lrt	

