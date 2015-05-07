flex scanner.l
gcc -g -O -c lex.yy.c 
gcc -std=c99 -g -O -c main.c
g++ -o dcc lex.yy.o  main.o  -lc -lm -ll
