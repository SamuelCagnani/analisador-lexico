CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -g
SRCS = src/main.c src/lexer.c src/token.c
OBJS = $(SRCS:.c=.o)
TARGET = lexer

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
