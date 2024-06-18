CC := gcc
CFLAGS := -Wall -Werror -Wextra -pedantic -std=gnu90

HEAD := sockets.h

todo_api_0: api_0.c
	@$(CC) $(CFLAGS) $(HEAD) $^ -o $@

clean:
	@$(RM) -f todo_api_*
