CC := gcc
CFLAGS := -Werror -Wall -Wextra -pedantic -std=gnu89
SRC := hls.c string.c
HEAD := hls.h
OBJ := $(SRC: %.c = %.o)
NAME := hls

all: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $(NAME)

debug: $(OBJ)
	$(CC) $(CFLAGS) $^ -g -o $(NAME)-debug

.PHONY: debug

$(SRC): $(HEAD)
