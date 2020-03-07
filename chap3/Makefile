P=pig_latin
OBJECTS=
CFLAGS =`pkg-config --cflags glib-2.0` -include allheads.h -g -Wall -O0 -std=gnu11 -D_GNU_SOURCE=1
LDLIBS=`pkg-config --libs glib-2.0` -lm -lfl
CC=gcc

LEXER_GEN_SRC=pl_lex.l
LEXER=pl_lex.yy.c

pig_latin: $(LEXER_GEN_SRC) $(OBJECTS)
	flex -o $(LEXER) $(LEXER_GEN_SRC)
	$(CC) $(CFLAGS) $(LEXER) $(LDLIBS) -o $@
