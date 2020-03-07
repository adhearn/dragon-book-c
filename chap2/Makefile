P=lexer
OBJECTS=
CFLAGS =`pkg-config --cflags glib-2.0` -include allheads.h -g -Wall -O0 -std=gnu11 -D_GNU_SOURCE=1
LDLIBS=`pkg-config --libs glib-2.0` -lm
CC=gcc

$(P): $(OBJECTS)
