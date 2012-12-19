#pragma once

int yyparse();
extern "C"{
void yyerror(const char* str);
}
int yylex();

#define NAME_TYPE 1
#define NUMBER_TYPE 2
#define ARRAY_TYPE 3
#define NEG_TYPE 4
#define MULT_TYPE 5
#define PLUS_TYPE 6
#define MINUS_TYPE 7
#define ARG_TYPE 8
#define ASSIGN_TYPE 9
#define SUM_TYPE 10
#define PART_TYPE 11
#define MAG_TYPE 12
#define EPS_TYPE 13

struct ast;

typedef struct ssa {
	int idx;
	struct ast* ast;
	struct ast* eps;
} ssa_t;

typedef struct ast {
	unsigned type;
	ssa_t *ssa;
} ast_t;

typedef struct name {
	ast_t ast;
	char* name;
} name_t;

typedef struct number {
	ast_t ast;
	int val;
} number_t;

typedef struct array {
	ast_t ast;
	char* name;
	int idx;
} array_t;

typedef struct neg {
	ast_t ast;
	ast_t* expr;
} neg_t;

typedef struct bin {
	ast_t ast;
	ast_t* a;
	ast_t* b;
} bin_t;

typedef struct arg {
	ast_t ast;
	char* name;
	ast_t* next;
} arg_t;

typedef struct sum {
	ast_t ast;
	ast_t* expr;
	ast_t* next;
} sum_t;

typedef struct assign {
	ast_t ast;
	name_t* dst;
	arg_t* args;
	ast_t* expr;
} assign_t;

typedef struct part {
	ast_t ast;
	int idx;
} part_t;

typedef struct mag {
	ast_t ast;
	ast_t* expr;
} mag_t;

typedef struct eps {
	ast_t ast;
} eps_t;

ast_t* name(char* p1);
ast_t* number(int p1);
ast_t* array(char* p1, int p2);
ast_t* neg(ast_t* neg);
ast_t* mult(ast_t* p1, ast_t* p2);
ast_t* plus(ast_t* p1, ast_t* p2);
ast_t* minus(ast_t* p1, ast_t* p2);
ast_t* arg(char *p1, ast_t* p2);
ast_t* assignment(ast_t* p1, ast_t* p2, ast_t *p3);
int endseq();
void execute(int p1);
int statement(ast_t* p1);

void recursiveFree(ast_t* ast);


