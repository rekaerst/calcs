CC = gcc
CFLAGS = -Wall -O3 -c $(shell pkg-config --cflags gtk+-3.0)
LDFLAGS = $(shell pkg-config --libs gtk+-3.0) -lm
SOURCE = main.c
OBJECTS = main.o
BINARY = calc

$(BINARY): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(BINARY)

$(OBJECTS): %.o: %.c 
	$(CC) $(SOURCE) $(CFLAGS)

clean:
	rm -rf *.o
	rm -rf $(BINARY)
