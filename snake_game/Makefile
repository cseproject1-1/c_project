CC = gcc
CFLAGS = -Wall -g
LDLIBS = -lncurses

all: snake

snake: snake.c
	$(CC) $(CFLAGS) -o snake snake.c $(LDLIBS)

clean:
	rm -f snake
