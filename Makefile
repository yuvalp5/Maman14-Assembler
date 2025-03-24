CC = gcc
CFLAGS = -Wall -ansi -pedantic -g

# Target executable
TARGET = assembler_test

# Source files
SRCS = test_first_pass.c inter/assembler_first_pass.c

# Object files
OBJS = $(SRCS:.c=.o)

# Header directories
INCLUDES = -I.

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJS) $(TARGET)

# Run the test
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
