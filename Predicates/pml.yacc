%{
#include "ast.h"
%}


%start ROOT

%token EQ
%token PLUS
%token MINUS
%token MULT
%token LPAREN
%token RPAREN
%token LBRACKET
%token RBRACKET
%token SEMICOLON
%token NUMBER
%token NAME
%token COMMA

%union {
    int intval;
    char* stringval;
	struct ast* astval; 
}

%type <intval> ROOT 
%type <intval> stmtseq 
%type <astval> statement
%type <astval> args
%type <astval> designator
%type <astval> expression
%type <astval> expr2
%type <astval> expr3
%type <astval> expr4
%type <astval> array
%type <intval> NUMBER
%type <stringval> NAME 


%%

ROOT:
	stmtseq { execute($1); } 
;

stmtseq: {$$ = endseq();}//empty
   | statement SEMICOLON stmtseq { $$ = statement($1); }
   ;

statement:
     designator LPAREN args RPAREN EQ expression { $$ = assignment($1, $3, $6); } 
;

args:
	 NAME COMMA args { $$ = arg($1, $3); }
	| NAME { $$ = arg($1, 0); }
	;


expression:
     expr2 { $$ = $1; } 
   ;

expr2:
     expr3 { $$ == $1; }
   | expr2 PLUS expr3 { $$ = plus($1, $3); }
   | expr2 MINUS expr3 { $$ = minus($1, $3); }
   ;

expr3:
     expr4 { $$ = $1; }
   | expr3 MULT expr4 { $$ = mult($1, $3); }
   ;

expr4:
     PLUS expr4 { $$ = $2; }
   | MINUS expr4 { $$ = neg($2); }
   | LPAREN expression RPAREN { $$ = $2; }
   | NUMBER { $$ = number($1); }
   | array
   ;

array:
     NAME LBRACKET NUMBER RBRACKET { $$ = array($1, $3); }
   ;

designator:
     NAME { $$ = name($1); }
   ;

