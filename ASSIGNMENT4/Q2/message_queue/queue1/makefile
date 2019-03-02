
CC=gcc

CFLAGS= -O0 -g 

CFILES= q1.c

SRCS= ${CFILES}
OBJS= ${CFILES:.c=.o}

build:	q1

clean:
	-rm -f *.o *.d
	-rm -f q1

q1:	q1.o
	$(CC)	$(LDFLAGS)	$(CFLAGS)	-o	$@	$@.o	-lrt

