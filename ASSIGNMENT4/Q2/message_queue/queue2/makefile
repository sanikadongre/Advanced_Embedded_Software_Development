
CC=gcc

CFLAGS= -O0 -g 

CFILES= hwq2.c

SRCS= ${CFILES}
OBJS= ${CFILES:.c=.o}

build:	hwq2

clean:
	-rm -f *.o *.d
	-rm -f hwq2

hwq2:	hwq2.o
	$(CC)	$(LDFLAGS)	$(CFLAGS)	-o	$@	$@.o	-lrt

