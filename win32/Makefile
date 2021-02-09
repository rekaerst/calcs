CC = x86_64-w64-mingw32-gcc
CFLAGS = -Wall -c -mwindows
LDFLAGS = -lm
SOURCE = $(wildchar *.c)
OBJECTS = $(SOURCE:%.c=%.o)
BINARY = calc.exe

$(BINARY): main.o
	$(CC) $^ $(LDFLAGS) -o $@
clean:
	rm -rf *.o *.exe
