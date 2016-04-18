CFLAGS = -std=c99

all:
	gcc cpu/syscall.c -o bin/syscall $(CFLAGS)
