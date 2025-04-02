# Compiler binary (cc is also a symlink to gcc)
CC = gcc

# Flags
CFLAGS = -Wall -ansi -pedantic 
LFLAGS = 

# Directories
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin

# Local includes
INCLUDES = -I. -I./shared -I./inter

# Source files
SRCS = $(wildcard *.c inter/*.c shared/*.c)

# Corrosponding object files
OBJS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(notdir $(SRCS)))

# Final executable
TARGET = $(BIN_DIR)/bin

# Link object files
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# Compile source files
$(OBJ_DIR)/%.o: $(wildcard */%.c)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Build everything
all: $(TARGET)

# Clean build files
clean:
	rm -rf $(BUILD_DIR)/*

# Run program
run: $(TARGET)
	./$(TARGET) $(ARGS)

.PHONY: all clean run
