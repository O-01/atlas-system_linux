CC := gcc
CFLAGS := -Wall -Werror -Wextra -pedantic

BUILD_DIR := build
INC_DIR := include
SRC_DIR := src
FH_NAME := file_header
FH_DIR := $(SRC_DIR)/$(FH_NAME)

SRC := $(filter-out src/1.c src/2.c, $(wildcard $(SRC_DIR)/*.c))
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

0-hreadelf-debug:
	$(CC) $(CFLAGS) $^ -g -o $@-debug

1-hreadelf: $(SRC_DIR)/1.c
	$(CC) $(CFLAGS) -I$(INC_DIR) $^ -o $@

2-hreadelf: $(SRC_DIR)/2.c
	$(CC) $(CFLAGS) -I$(INC_DIR) $^ -o $@

clean:
	$(RM) -f *-hreadelf *-hreadelf-debug
	$(RM) -r $(BUILD_DIR)

.PHONY: clean

$(SRC): $(HEAD)
