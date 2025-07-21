# temporary makefile commands

.PHONY : all

all : debug

.PHONY : debug

debug :
	cc -Wall -Wextra -Werror -std=c99 -O0 -ggdb3 -c ./src/x11/gf.c -o gf.o -DDEBUG
	ar rcs libgf.a gf.o

.PHONY : release

release :
	cc -Wall -Wextra -Werror -std=c99 -O3 -S -c ./src/x11/gf.c -o gf.o -DRELEASE
	ar rcs libgf.a gf.o
