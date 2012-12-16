#!/bin/sh
lex pml.lex
yacc pml.yacc -d
g++ lex.yy.c y.tab.c main.c ast.c -o pml
