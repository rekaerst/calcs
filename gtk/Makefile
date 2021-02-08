CC=gcc
SOURCE=main.c
BINARY=main

main: ${SOURCE}
	${CC} ${SOURCE} -g -o ${BINARY} -fPIC ${shell pkg-config --cflags --libs gtk+-3.0} -lm
