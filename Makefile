LIBDIR = lib
LIBFILES = lib/*.c
BINDIR = bin
CFLAGS = -std=c99 -pthread -I$(LIBDIR) -lm

.PHONY: all mem cpu net

all: cpu mem net

cpu:
	gcc cpu/syscall.c $(LIBFILES) -o $(BINDIR)/syscall $(CFLAGS)
	gcc cpu/proccall.c $(LIBFILES) -o $(BINDIR)/proccall $(CFLAGS)
	gcc cpu/context_switch.c $(LIBFILES) -o $(BINDIR)/context_switch $(CFLAGS)
	gcc cpu/thread.c $(LIBFILES) -o $(BINDIR)/thread $(CFLAGS)
	gcc cpu/measurement.c $(LIBFILES) -o $(BINDIR)/measure $(CFLAGS)
	gcc cpu/proc_create.c $(LIBFILES) -o $(BINDIR)/proc_create $(CFLAGS)
	gcc cpu/thread_create.c $(LIBFILES) -o $(BINDIR)/thread_create $(CFLAGS)

mem:
	gcc mem/mem_latency.c $(LIBFILES) -o $(BINDIR)/mem_latency $(CFLAGS)
	gcc -O0 mem/mem_bandwidth.c $(LIBFILES) -funroll-loops -o $(BINDIR)/mem_bandwidth $(CFLAGS) --param max-unroll-times=128
	gcc mem/mem_pagefault.c $(LIBFILES) -o $(BINDIR)/mem_pagefault $(CFLAGS)
	gcc mem/test.c $(LIBFILES) -o $(BINDIR)/test $(CFLAGS)

net:
	gcc net/server.c $(LIBFILES) -o $(BINDIR)/net_server $(CFLAGS)
	gcc net/rtt_client.c $(LIBFILES) -o $(BINDIR)/net_rtt_client $(CFLAGS)
