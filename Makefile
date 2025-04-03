# Compiler binary (cc is also a symlink to gcc)
CC = gcc

# Flags
CFLAGS = -Wall -ansi -pedantic
LFLAGS = 

# Source files - wrapper.c inter/pre_assembler.c inter/assembler_first_pass.c inter/assembler_second_pass.c shared/utils.c shared/types.c
SRCS = $(wildcard *.c) $(wildcard inter/*.c) $(wildcard shared/*.c)

# Object files
OBJS = $(SRCS:.c=.o)

# Header directories for lookup
INCLUDES = -I. -I./shared -I./inter

# Final executable
TARGET = io/bin

# Link object files using Makefile pattern rules
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDES) -o $@ $^

# Compile source files using Makefile pattern rules
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Build targets
all: $(TARGET)

# Clean option - testing files not removed for debug pruposes
clean: 
	rm -f $(OBJS) $(TARGET)
	rm -f io/*.am io/*.ob io/*.ext io/*.ent

# Run the test
run: $(TARGET)
	./$(TARGET) $(ARGS)

# Fake targets
.PHONY: all clean run