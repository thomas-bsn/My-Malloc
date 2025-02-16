CC = gcc
CFLAGS = -pedantic -Werror -Wall -Wextra -Wvla -std=c99 -D_DEFAULT_SOURCE -fsanitize=address -g
SRC = main.c utils.c allocator.c
OBJS = $(SRC:.c=.o)

out: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ 

clean:
	$(RM) *.o *.a out

