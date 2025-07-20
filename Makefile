# temporary makefile command
all :
	cc -Wall -Wextra -Werror -c ./src/x11/gf.c -o gf.o
	ar rcs libgf.a gf.o
