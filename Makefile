all:
	flex prac.l;
	yacc -d prac.y;
	cc lex.yy.c y.tab.c envar.c cmdcode.c main.c -o run
clean:
	rm -rf *o run