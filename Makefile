PLATFORM=$(shell uname -s)
ARCH=$(shell uname -m)

CFLAGS=-Wall -ansi -g -ggdb
CFLAGS+= -I/usr/X11/include
LFLAGS= -L/usr/X11/lib -lX11

SRC=main.c hitable.c material.c vec3.c ray.c
OBJ:=$(SRC:.c=.o)
BIN=raytracer

# Run each test program and echo exit code
$(BIN): $(OBJ)
	$(CC) -o $@ $^  $(LFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -v *.o

# Targets clean and all are not real artifacts
.PHONY: clean all

# Prevent automatic removal of intermediate files
.PRECIOUS:
