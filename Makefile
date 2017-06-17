PLATFORM=$(shell uname -s)
ARCH=$(shell uname -m)

#CC=/usr/local/bin/gcc-7

CFLAGS=-Wall -ansi -g -ggdb -O3 -pedantic -std=c99
CFLAGS+= -I/usr/X11/include
#CFLAGS+= -pg -fprofile-arcs -ftest-coverage
LFLAGS= -L/usr/X11/lib -lX11
#LFLAGS+= -pg -fprofile-arcs -ftest-coverage

SRC=main.c hitable.c material.c vec3.c ray.c camera.c
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
