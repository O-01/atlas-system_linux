CC := gcc
CFLAGS := -Wall -Werror -Wextra -pedantic

BUILD_DIR := build
INC_DIR := include
SRC_DIR := src
FH_NAME := file_header
FH_DIR := $(SRC_DIR)/$(FH_NAME)

SRC := $(wildcard $(SRC_DIR)/*.c)
FH_SRC := $(wildcard $(FH_DIR)/*.c)

OBJ := $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(notdir $(SRC))))
FH_OBJ := $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(notdir $(FH_SRC))))

HEAD := $(INC_DIR)/hreadelf.h

all: 0-hreadelf 1-hreadelf 2-hreadelf

vpath %.c $(SRC_DIR) $(FH_DIR)

$(BUILD_DIR)/%.o : %.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -I$(INC_DIR) $< -o $@

0-hreadelf: $(OBJ) $(FH_OBJ)
	$(CC) $(CFLAGS) $^ -o $@

1-hreadelf: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

2-hreadelf: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	$(RM) -f 0-hreadelf 1-hreadelf 2-hreadelf
	$(RM) -r $(BUILD_DIR)

.PHONY: clean

$(SRC): $(HEAD)
