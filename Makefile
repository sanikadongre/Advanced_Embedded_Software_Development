CC=gcc

CFLAGS= -O0 -g
 
CFILES= hwq2.c

SRCS= ${CFILES}
OBJS= ${CFILES:.c=.o}

build:	hwq2.elf

clean:
	-rm -f *.o *.d
	-rm -f hwq2.elf

hwq2.elf:	hwq2.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o -lm
