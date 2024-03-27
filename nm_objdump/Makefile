CC := gcc
CFLAGS := -Wall -Werror -Wextra -pedantic -std=gnu89

BUILD_DIR := build
INC_DIR := include
SRC_DIR := src

NM_DIR := $(SRC_DIR)/hnm
OBJDUMP_DIR := $(SRC_DIR)/hobjdump

SRC := $(wildcard $(SRC_DIR)/*.c)
NM_SRC := $(SRC) $(wildcard $(NM_DIR)/*.c)

OBJ := $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(notdir $(SRC))))
NM_OBJ := $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(notdir $(NM_SRC))))

HEAD := $(INC_DIR)/hnm_hobjdump.h

all: hnm hobjdump

vpath *.c $(SRC_DIR) $(NM_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -I$(INC_DIR) $< -o $@

hnm: $(NM_OBJ)
	$(CC) $(CFLAGS) $^ -o $@

hnm_h4x:
	@echo '#!/bin/bash' > $@
	@echo '( exec -a "./hnm" nm -p $$1 )' >> $@
	chmod u+x -v $@

hobjdump:
	@echo '#!/bin/bash' > $@
	@echo '( exec -a "./hobjdump" objdump -sf $$1 )' >> $@
	chmod u+x -v $@

clean:
	$(RM) -f hnm hobjdump
	$(RM) -r $(BUILD_DIR)

.PHONY: clean

$(SRC): $(HEAD)
