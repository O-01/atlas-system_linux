CC := gcc
CFLAGS := -Wall -Werror -Wextra -pedantic -std=gnu89

BUILD_DIR := build
INC_DIR := include
SRC_DIR := src
FH_NAME := file_header
SH_NAME := section_header
PH_NAME := program_header

FH_DIR := $(SRC_DIR)/$(FH_NAME)
SH_DIR := $(SRC_DIR)/$(SH_NAME)
PH_DIR := $(SRC_DIR)/$(PH_NAME)

SRC := $(wildcard $(SRC_DIR)/*.c)
FH_SRC := $(SRC) $(wildcard $(FH_DIR)/*.c)
SH_SRC := $(SRC) $(wildcard $(SH_DIR)/*.c)
PH_SRC := $(SRC) $(wildcard $(PH_DIR)/*.c)

OBJ := $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(notdir $(SRC))))
FH_OBJ := $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(notdir $(FH_SRC))))
SH_OBJ := $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(notdir $(SH_SRC))))
PH_OBJ := $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(notdir $(PH_SRC))))

HEAD := $(INC_DIR)/hreadelf.h

all: 0-hreadelf 1-hreadelf 2-hreadelf

vpath %.c $(SRC_DIR) $(FH_DIR) $(SH_DIR) $(PH_DIR)

$(BUILD_DIR)/%.o : %.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -I$(INC_DIR) $< -o $@

0-hreadelf: $(FH_OBJ)
	$(CC) $(CFLAGS) $^ -o $@

0-hreadelf-debug: $(FH_OBJ)
	$(CC) $(CFLAGS) $^ -g -o $@-debug

1-hreadelf: $(SH_OBJ)
	$(CC) $(CFLAGS) $^ -o $@

1-hreadelf-debug: $(SH_OBJ)
	$(CC) $(CFLAGS) $^ -g -o $@-debug

2-hreadelf: $(PH_OBJ)
	$(CC) $(CFLAGS) $^ -o $@

2-hreadelf-debug: $(PH_OBJ)
	$(CC) $(CFLAGS) $^ -g -o $@-debug

clean:
	$(RM) -f *-hreadelf *-hreadelf-debug
	$(RM) -r $(BUILD_DIR)

.PHONY: clean

$(SRC): $(HEAD)
