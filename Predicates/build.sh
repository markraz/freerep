#!/bin/sh
lex pml.lex
yacc pml.yacc -d
g++ lex.yy.c y.tab.c main.cpp ast.cpp args.cpp expr.cpp ssa.cpp ntree.cpp -o pml
