# Compiler binary (cc is also a symlink to gcc)
CC = gcc

# Flags
CFLAGS = -Wall -ansi -pedantic -g
LFLAGS = 

# Source files
SRCS = $(wildcard *.c) $(wildcard inter/*.c)
#wrapper.c inter/pre_assembler.c inter/assembler_first_pass.c inter/assembler_second_pass.c

# Object files
OBJS = $(SRCS:.c=.o)

# Final executable
TARGET = io/bin

# Header directories for lookup
INCLUDES = -I. -I./shared I./inter 

# Build targets
all: $(TARGET)
#test: $(TARGET)

# Clean option
clean: 
	rm -f $(OBJS) $(TARGET)

# Fake targets
.PHONY: all test clean

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Run the test
run: $(TARGET)
	./$(TARGET)
