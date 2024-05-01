CC := gcc
CFLAGS := -Wall -Werror -Wextra -pedantic

INCDIR := include
SRCDIR := src
BUILDDIR := build

SRC := $(wildcard $(SRCDIR)/*.c)
OBJ := $(addprefix $(BUILDDIR)/, $(patsubst %.c, %.o, $(notdir $(SRC))))
HEAD := $(INCDIR)/strace.h

$(BUILDDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c -I$(INCDIR) $< -o $@

strace_0: $(BUILDDIR)/strace_0.o
	$(CC) $(CFLAGS) $^ -o $@

strace_1: $(BUILDDIR)/strace_1.o
	$(CC) $(CFLAGS) $^ -o $@

strace_2: $(BUILDDIR)/strace_2.o
	$(CC) $(CFLAGS) $^ -o $@

clean:
	@$(RM) -rf build
	@$(RM) -f strace_*

.PHONY: clean

$(SRC): $(HEAD)
