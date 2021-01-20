# basic
CC=gcc
CFLAGS=-std=c18 -pedantic -Wall -Werror -g

# dirs
INCDIR=include
OBJDIR=obj

all: main.o
	$(CC) $(OBJDIR)/*.o -o bmprot $(CFLAGS)
	./bmprot

main.o: ./main.c
	$(CC) -c ./main.c -o $(OBJDIR)/main.o $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o
