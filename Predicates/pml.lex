%{
#include "ast.h"
#include "y.tab.h"
extern YYSTYPE yylval;
%}
%%
"="      { return EQ; }
"+"      { return PLUS; }
"-"      { return MINUS; }
"*"      { return MULT; }
")"      { return RPAREN; }
"("      { return LPAREN; }
"["		 { return LBRACKET; }
"]"		 { return RBRACKET; }
";"		 { return SEMICOLON; }
","		 { return COMMA; }
[0-9]+   { yylval.intval = atoi(yytext); return NUMBER; }
[a-z]+   { yylval.stringval = strdup(yytext); return NAME; }   
\        { ; }
\n       { ; }
\t       { ; }
"//".*\n { ; }
.        { yyerror("illegal token"); }
%%



