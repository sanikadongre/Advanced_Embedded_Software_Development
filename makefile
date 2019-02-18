
CC=gcc

CFLAGS= -O0 -g 

CFILES= sanika_timer.c

SRCS= ${CFILES}
OBJS= ${CFILES:.c=.o}

build:	sanika_timer

clean:
	-rm -f *.o *.d
	-rm -f sanika_timer

sanika_timer:	sanika_timer.o
	$(CC)	$(LDFLAGS)	$(CFLAGS)	-o	$@	$@.o

