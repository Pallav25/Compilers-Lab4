CC = gcc
CFLAGS =  -Wall 
LEX = flex
YACC = bison
YACCFLAGS = -d

.SUFFIXES:
 
.PHONY: all clean

all: sample.txt parser
	@./parser < sample.txt 

parser: lex.yy.c y.tab.c polyutils.c
	@$(CC) $(CFLAGS) -o parser lex.yy.c poly.tab.c  polyutils.c -lfl -lm

lex.yy.c: poly.l
	@$(LEX) poly.l

y.tab.c: poly.y
	@$(YACC) $(YACCFLAGS) poly.y

clean:
	@rm -f parser lex.yy.c poly.tab.c poly.tab.h
	