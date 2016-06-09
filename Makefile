LIBDIR = lib
LIBFILES = lib/*.c
BINDIR = bin
CFLAGS = -std=c99 -pthread -I$(LIBDIR) -lm
COMPILER = colorgcc

.PHONY: all mem cpu net fs

all: cpu mem net fs

cpu:
	$(COMPILER) cpu/syscall.c $(LIBFILES) -o $(BINDIR)/syscall $(CFLAGS)
	$(COMPILER) cpu/proccall.c $(LIBFILES) -o $(BINDIR)/proccall $(CFLAGS)
	$(COMPILER) cpu/context_switch.c $(LIBFILES) -o $(BINDIR)/context_switch $(CFLAGS)
	$(COMPILER) cpu/thread.c $(LIBFILES) -o $(BINDIR)/thread $(CFLAGS)
	$(COMPILER) cpu/measurement.c $(LIBFILES) -o $(BINDIR)/measure $(CFLAGS)
	$(COMPILER) cpu/proc_create.c $(LIBFILES) -o $(BINDIR)/proc_create $(CFLAGS)
	$(COMPILER) cpu/thread_create.c $(LIBFILES) -o $(BINDIR)/thread_create $(CFLAGS)

mem:
	$(COMPILER) mem/mem_latency.c $(LIBFILES) -o $(BINDIR)/mem_latency $(CFLAGS)
	$(COMPILER) -O0 mem/mem_bandwidth.c $(LIBFILES) -funroll-loops -o $(BINDIR)/mem_bandwidth $(CFLAGS) --param max-unroll-times=128
	$(COMPILER) mem/mem_pagefault.c $(LIBFILES) -o $(BINDIR)/mem_pagefault $(CFLAGS)
	$(COMPILER) mem/test.c $(LIBFILES) -o $(BINDIR)/test $(CFLAGS)

net:
	$(COMPILER) net/server.c $(LIBFILES) -o $(BINDIR)/net_server $(CFLAGS)
	$(COMPILER) net/rtt_client.c $(LIBFILES) -o $(BINDIR)/net_rtt_client $(CFLAGS)
	$(COMPILER) net/conn_client.c $(LIBFILES) -o $(BINDIR)/net_conn_client $(CFLAGS)
	$(COMPILER) net/icmp_client.c $(LIBFILES) -o $(BINDIR)/net_icmp_client $(CFLAGS)
	$(COMPILER) net/file_server.c $(LIBFILES) -o $(BINDIR)/net_file_server $(CFLAGS)
	$(COMPILER) net/file_client.c $(LIBFILES) -o $(BINDIR)/net_file_client $(CFLAGS)

fs:
	$(COMPILER) fs/cache.c $(LIBFILES) -o $(BINDIR)/fs_cache $(CFLAGS)
