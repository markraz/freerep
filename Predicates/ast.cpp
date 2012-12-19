#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "ast.h"

using namespace std;

static vector<ast_t*> statements;

vector<ast_t*>* getStatements(){
	return &statements;
}

void execute(int p1){
//	printf("execute\n");
}

ast_t* plus(ast_t* p1, ast_t* p2){
//	printf("plus %d %d\n", p1, p2);
	bin_t* ret = (bin_t*)malloc(sizeof(bin_t));
	ret->ast.type = PLUS_TYPE;
	ret->ast.ssa = 0;
	ret->a = p1;
	ret->b = p2;
	return (ast_t*)ret;
}

ast_t* minus(ast_t* p1, ast_t* p2){
//	printf("minus\n");
	bin_t* ret = (bin_t*)malloc(sizeof(bin_t));
	ret->ast.type = MINUS_TYPE;
	ret->ast.ssa = 0;
	ret->a = p1;
	ret->b = p2;
	return (ast_t*)ret;
}

ast_t* mult(ast_t* p1, ast_t* p2){
//	printf("mult\n");
	bin_t* ret = (bin_t*)malloc(sizeof(bin_t));
	ret->ast.type = MULT_TYPE;
	ret->ast.ssa = 0;
	ret->a = p1;
	ret->b = p2;
	return (ast_t*)ret;
}	

ast_t* assignment(ast_t* p1, ast_t* p2, ast_t *p3){
//	printf("assignment %s %p %p\n", p1->name, p2, p3);
	assign_t* ret = (assign_t*)malloc(sizeof(assign_t));
	ret->ast.type = ASSIGN_TYPE;
	ret->ast.ssa = 0;
	ret->dst = (name_t*)p1;
	ret->args = (arg_t*)p2;
	ret->expr = p3;
	return (ast_t*)ret;
}

int statement(ast_t* p1){
	//printf("statement\n");
	statements.push_back(p1);
	return 0;
}

ast_t* name(char* p1){
//	printf("name %s\n", p1);
	name_t *ret = (name_t*)malloc(sizeof(name_t));
	ret->ast.ssa = 0;
	ret->ast.type = NAME_TYPE;
	ret->name = p1;
	return (ast_t*)ret;
}

ast_t* neg(ast_t *p1){
//	printf("neg\n");
	neg_t *ret = (neg_t*)malloc(sizeof(neg_t));
	ret->ast.type = NEG_TYPE;
	ret->ast.ssa = 0;
	ret->expr = p1;
	return (ast_t*)ret;
}

void seq(char *p1){
	printf("seq\n");
}

ast_t* number(int p1){
//	printf("number %d\n", p1);
	number_t* ret = (number_t*)malloc(sizeof(number_t));
	ret->ast.type = NUMBER_TYPE;
	ret->ast.ssa = 0;
	ret->val = p1;
	return (ast_t*)ret;
}

ast_t* array(char *p1, int p2){
//	printf("array %s %d\n", p1, p2);
	array_t* ret = (array_t*)malloc(sizeof(array_t));
	ret->ast.type = ARRAY_TYPE;
	ret->ast.ssa = 0;
	ret->name = p1;
	ret->idx = p2;
	return (ast_t*)ret;
}

ast_t* arg(char *p1, ast_t* p2){
//	printf("args %s\n", p1);
	arg_t* ret = (arg_t*)malloc(sizeof(arg_t));
	ret->ast.type = ARG_TYPE;
	ret->ast.ssa = 0; 
	ret->name = p1;
	ret->next = p2;
	return (ast_t*)ret;
}

int endseq(){
//	printf("endseq\n");
	return 0;
}

void recursiveFree(ast_t* ast){
	switch(ast->type){
		case NUMBER_TYPE: break;
		case ARRAY_TYPE: break;
		case PLUS_TYPE: //fall through
		case MINUS_TYPE: //fall through
		case MULT_TYPE: 
			{
				bin_t* bin = (bin_t*)ast;
				recursiveFree(bin->a);
				recursiveFree(bin->b);
			}
			break;
		case NEG_TYPE: 
			{
				neg_t* neg = (neg_t*)ast;
				recursiveFree(neg->expr);
			}
			break;
		default:
			printf("Fail\n");
	}
	if(ast->ssa)
		free(ast->ssa);
	free(ast);
}
