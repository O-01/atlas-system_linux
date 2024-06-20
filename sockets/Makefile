CC := gcc
CFLAGS := -Wall -Werror -Wextra -pedantic

UTIL := util.c
UTIL_REST := util_REST.c

todo_api_0: api_0.c $(UTIL)
	@$(CC) $(CFLAGS) $^ -o $@

todo_api_1: api_1.c $(UTIL)
	@$(CC) $(CFLAGS) $^ -o $@

todo_api_2: api_2.c $(UTIL)
	@$(CC) $(CFLAGS) $^ -o $@

todo_api_3: api_3.c $(UTIL)
	@$(CC) $(CFLAGS) $^ -o $@

todo_api_4: api_4.c $(UTIL) $(UTIL_REST)
	@$(CC) $(CFLAGS) $^ -o $@

todo_api_5: api_5.c $(UTIL) $(UTIL_REST)
	@$(CC) $(CFLAGS) $^ -o $@

clean:
	@$(RM) -f todo_api_*
