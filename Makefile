# basic
CC=gcc
CFLAGS=-std=c18 -pedantic -Wall -Werror -g

# dirs
INCDIR=include
OBJDIR=obj

all: main.o bmp.o
	$(CC) $(OBJDIR)/*.o -o bmprot $(CFLAGS)

run: all
	./bmprot BMPs/FontDialog.bmp

main.o: ./main.c
	$(CC) -c ./main.c -o $(OBJDIR)/main.o $(CFLAGS)

bmp.o: ./bmp.c
	$(CC) -c ./bmp.c -o $(OBJDIR)/bmp.o $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o
