CC = gcc
CFLAGS = -Wall -Wextra -std=c99


SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

island: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f island $(OBJS)