
CC=gcc

CFLAGS= -O0 -g 

CFILES= pipes.c

SRCS= ${CFILES}
OBJS= ${CFILES:.c=.o}

build:	pipes

clean:
	-rm -f *.o *.d
	-rm -f pipes

pipes:	pipes.o
	$(CC)	$(LDFLAGS)	$(CFLAGS)	-o	$@	$@.o	-lrt

