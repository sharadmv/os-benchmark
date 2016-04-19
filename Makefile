CFLAGS = -std=c99 -pthread
LIBFILES = cpu/util.c cpu/init.c

all:
	gcc cpu/syscall.c $(LIBFILES) -o bin/syscall $(CFLAGS)
	gcc cpu/proccall.c $(LIBFILES) -o bin/proccall $(CFLAGS)
	gcc cpu/context_switch.c $(LIBFILES) -o bin/context_switch $(CFLAGS)
	gcc cpu/thread.c $(LIBFILES) -o bin/thread $(CFLAGS)
	gcc cpu/measurement.c $(LIBFILES) -o bin/measure $(CFLAGS)
	gcc cpu/proc_create.c $(LIBFILES) -o bin/proc_create $(CFLAGS)
	gcc cpu/thread_create.c $(LIBFILES) -o bin/thread_create $(CFLAGS)
