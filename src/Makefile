CC ?= gcc
CFLAGS ?= -Wall -O2
EXEC := main

OBJS_main := main.o parsing.o process.o

.PHONY: clean

all: $(EXEC)

main: $(OBJS_main)
	$(CC) $(CFLAG) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

% : %.o
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o
