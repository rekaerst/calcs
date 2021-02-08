CC=gcc
SOURCE=main.c
BINARY=main

main: ${SOURCE}
	${CC} ${SOURCE} -o ${BINARY} -fPIC ${shell pkg-config --cflags --libs gtk+-3.0}
