#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#include "y.tab.h"
#include "ast.h"

using namespace std;

vector<ast_t*>* getStatements();

extern "C"{
 
void yyerror(const char *str)
{
        fprintf(stderr,"error: %s\n",str);
}
 
int yywrap()
{
        return 1;
} 

} //Extern "C"

vector<char*> getArgs(arg_t* args){
	vector<char*> ret;
	while(args){
		if(args->ast.type != ARG_TYPE){
			printf("Compiler error %s\n", __func__);
		}
		ret.push_back(args->name);
		args = (arg_t*)args->next;
	}
	return ret;
}

void printPrototype(char* name, vector<char*> args){
	printf("double %s(", name);
	for(int i=0; i < args.size(); i++){
		if(i>0)
			printf(", ");
		printf("double *%s", args[i]);
	}
	printf(") {\n");
}

void addArg(char* name, vector<char*> *args){
	for(int i=0; i<args->size(); i++){
		if(strcmp(name,args->at(i))==0)
			return;
	}
	args->push_back(name);
}

void getUsedArgs(ast_t* expr, vector<char*> *args){
	switch(expr->type){
		case NUMBER_TYPE: break;
		case ARRAY_TYPE:
			{
				array_t* array = (array_t*)expr;
				addArg(array->name,args);
			} 
			break;
		case PLUS_TYPE: //fall through
		case MINUS_TYPE: //fall through
		case MULT_TYPE: 
			{
				bin_t* bin = (bin_t*)expr;
				getUsedArgs(bin->a,args);
				getUsedArgs(bin->b,args);
			}
			break;
		case NEG_TYPE: 
			{
				neg_t* neg = (neg_t*)expr;
				getUsedArgs(neg->expr,args);
			}
			break;
		default: printf("Fail\n"); 
	}
}

void checkUsedArgs(vector<char*> *usedargs, vector<char*> *args){
	for(int i=0; i < usedargs->size(); i++){
		bool found=false;
		for(int j=0; j < args->size(); j++){
			if(strcmp(usedargs->at(i),args->at(j))==0){
				found = true;
				break;
			}
		}
		if(!found){
			printf("Undeclared variable %s\n", usedargs->at(i));
		}
	}
}

bool reduceExpression(ast_t* expr, ast_t** prt){
	switch(expr->type){
		case NUMBER_TYPE: break;
		case ARRAY_TYPE: break;
		case PLUS_TYPE: //fall through
		case MINUS_TYPE: //fall through
		case MULT_TYPE: 
			{
				bin_t* bin = (bin_t*)expr;
				if(bin->a->type == NUMBER_TYPE &&	
				   bin->b->type == NUMBER_TYPE){
				   	double res = ((number_t*)bin->a)->val * ((number_t*)bin->b)->val;
					if(expr->type==MINUS_TYPE)
						res = ((number_t*)bin->a)->val - ((number_t*)bin->b)->val;
					if(expr->type==PLUS_TYPE)
						res = ((number_t*)bin->a)->val + ((number_t*)bin->b)->val;
				  	number_t *num = (number_t*)malloc(sizeof(number_t));
					num->ast.type = NUMBER_TYPE;
					num->val = res;
					*prt = (ast_t*)num;
   				    return true;
				}
				return reduceExpression(bin->a,&bin->a) || reduceExpression(bin->b,&bin->b);
			}
			break;
		case NEG_TYPE: 
			{
				neg_t* neg = (neg_t*)expr;
				if(neg->expr->type == NUMBER_TYPE){
					double res = -((number_t*)neg->expr)->val;
				  	number_t *num = (number_t*)malloc(sizeof(number_t));
					num->ast.type = NUMBER_TYPE;
					num->val = res;
					*prt = (ast_t*)num;
					return true;
				}
				return reduceExpression(neg->expr,&neg->expr);
			}
			break;
		default: printf("Fail %d\n", expr->type); 
	}
	return 0;
}

void printExprInexact(ast_t* expr){
	switch(expr->type){
		case NUMBER_TYPE: 
			{
				printf("%d");
			}				
			break;
		case ARRAY_TYPE: 
			{
				array_t* array = (array_t*)expr;
				printf("%s[%d]", array->name, array->idx);	
			}
			break;
		case PLUS_TYPE: 
			{
				bin_t* bin = (bin_t*)expr;
				printf("(");
				printExprInexact(bin->a);
				printf("+");
				printExprInexact(bin->b);
				printf(")");
			}
			break;

		case MINUS_TYPE: 
			{
				bin_t* bin = (bin_t*)expr;
				printf("(");
				printExprInexact(bin->a);
				printf("-");
				printExprInexact(bin->b);
				printf(")");
			}
			break;

		case MULT_TYPE: 
			{
				bin_t* bin = (bin_t*)expr;
				printf("(");
				printExprInexact(bin->a);
				printf("*");
				printExprInexact(bin->b);
				printf(")");
			}
			break;
		case NEG_TYPE: 
			{
				neg_t* neg = (neg_t*)expr;
			}
			break;
		default: printf("Fail %d\n", expr->type); 
	}
}

void compile(ast_t* ast){
	char* fname;
	vector<char*> args;
	vector<char*> usedargs;

	//printf("Compiling %p\n", ast);
	if(ast->type != ASSIGN_TYPE){
		printf("Root node not assignment\n");
		return;	
	}
	assign_t *assign = (assign_t*)ast;
	fname = assign->dst->name;
	args = getArgs(assign->args);

	getUsedArgs(assign->expr,&usedargs);

	checkUsedArgs(&usedargs,&args);
	//while(reduceExpression(assign->expr,&assign->expr)){printf("Reduce....\n");}

	printPrototype(fname,args);

	//Normal epsilon calculation
	printf("\tdouble det=");
	printExprInexact(assign->expr);
	printf(";\n");


	printf("}\n");
}
  
int main()
{
        if(yyparse()){
			printf("Parse failed\n");
			return 0;
		}

		vector<ast_t*>* statements = getStatements();

		for(int i=0; i < statements->size(); i++){
			compile(statements->at(i));
		}
} 


