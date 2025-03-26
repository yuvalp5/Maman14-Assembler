# Compiler binary (cc is also a symlink to gcc)
CC = gcc

# Flags
CFLAGS = -Wall -ansi -pedantic
LFLAGS = 

# Source files
SRCS = $(wildcard *.c) $(wildcard inter/*.c) $(wildcard shared/*.c)
#wrapper.c inter/pre_assembler.c inter/assembler_first_pass.c inter/assembler_second_pass.c shared/utils.c shared/types.c

# Header directories for lookup
INCLUDES = -I. -I./shared -I./inter

# Object files
OBJS = $(SRCS:.c=.o)

# Final executable
TARGET = io/bin

# Link object files using Makefile pattern rules
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(INCLUDES) -o $@ $^

# Compile source files using Makefile pattern rules
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Build targets
all: $(TARGET)
#test: $(TARGET)

# Clean option
clean: 
	rm -f $(OBJS) $(TARGET)

# Run the test
run: $(TARGET)
	./$(TARGET)

# Fake targets
.PHONY: all test clean