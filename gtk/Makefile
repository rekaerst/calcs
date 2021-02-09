CC = gcc
CFLAGS = -Wall -c $(shell pkg-config --cflags gtk+-3.0)
LDFLAGS = $(shell pkg-config --libs gtk+-3.0) -lm
SOURCE = $(wildchar *.c)
OBJECTS = $(SOURCE:%.c=%.o)
BINARY = calc

$(BINARY): main.o
	$(CC) $^ $(LDFLAGS) -o $@
