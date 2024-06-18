CC := gcc
CFLAGS := -Wall -Werror -Wextra -pedantic

UTIL := util.c

todo_api_0: api_0.c $(UTIL)
	@$(CC) $(CFLAGS) $^ -o $@

todo_api_1: api_1.c $(UTIL)
	@$(CC) $(CFLAGS) $^ -o $@ -g

clean:
	@$(RM) -f todo_api_*
