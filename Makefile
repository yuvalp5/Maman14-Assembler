CC = gcc
CFLAGS = -Wall -ansi -pedantic -I.
SRCS = wrapper.c shared/utils.c shared/types.c inter/pre_assembler.c inter/assembler_first_pass.c inter/assembler_second_pass.c
OBJS = $(SRCS:.c=.o)
TARGET = assembler

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
