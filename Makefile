P=chap2-print
OBJECTS=
CFLAGS = -include allheads.h -g -Wall -O3 -std=gnu11 -D_GNU_SOURCE=1
LDLIBS=-lm
CC=gcc

$(P): $(OBJECTS)
