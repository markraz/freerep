#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

using namespace std;

#include "y.tab.h"
#include "ast.h"
#include "main.h"

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

void printPrototype(char* name, vector<char*> args){
	printf("double %s(", name);
	for(int i=0; i < args.size(); i++){
		if(i>0)
			printf(", ");
		printf("double *%s", args[i]);
	}
	printf(") {\n");
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
					num->ast.ssa = 0;
					num->val = res;
					*prt = (ast_t*)num;
					recursiveFree(expr);
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
					num->ast.ssa = 0;
					num->val = res;
					*prt = (ast_t*)num;
					recursiveFree(expr);
					return true;
				}
				return reduceExpression(neg->expr,&neg->expr);
			}
			break;
		default: printf("Fail %d\n", expr->type); 
	}
	return 0;
}

//code or names
int printExprInexact(ast_t* expr, int idx, bool code){
	switch(expr->type){
		case NUMBER_TYPE: 
			{
				number_t* number = (number_t*)expr;
				if(!code)
					printf("%d",number->val);
			}				
			break;
		case ARRAY_TYPE: 
			{
				array_t* array = (array_t*)expr;
				if(!code)
					printf("%s[%d]", array->name, array->idx);	
			}
			break;
		case PLUS_TYPE: //fall through
		case MINUS_TYPE: //fall through
		case MULT_TYPE: 
			{
				bin_t* bin = (bin_t*)expr;
				if(code){
					int aidx = printExprInexact(bin->a,idx+1,1);
					int bidx = printExprInexact(bin->b,aidx+1,1);
					printf("\tepserr+=(ABS(");
					printExprInexact(bin->a,idx+1,0);
					printf(")+ABS(");
					printExprInexact(bin->b,aidx+1,0);
					printf("))*EPS_DBL;\n");
					printf("\tdouble partial%d=", idx);
					printExprInexact(bin->a,idx+1,0);
					if(expr->type==MULT_TYPE)
						printf("*");
					if(expr->type==PLUS_TYPE)
						printf("+");
					if(expr->type==MINUS_TYPE)
						printf("-");
					idx = printExprInexact(bin->b,aidx+1,0);
					printf(";\n");
				}else{
					printf("partial%d",idx);
				}
			}
			break;
		case NEG_TYPE: 
			{
				neg_t* neg = (neg_t*)expr;
				printf("ENOIMPL-");
				printExprInexact(neg->expr,idx,code);
			}
			break;
		default: printf("Fail %d\n", expr->type); 
	}
	return idx;
}

void isPredictable(ast_t* expr, int sign){
	switch(expr->type){
		case NUMBER_TYPE: 
			{
				number_t* number = (number_t*)expr;
				if((sign==0 && number->val==0) ||  
					(sign==1 && number->val>=0) ||
					(sign==-1 && number->val<=0)) 
						printf("true");
					else
						printf("false");
			}				
			break;
		case ARRAY_TYPE: 
			{
				array_t* array = (array_t*)expr;
				if(sign==0)
					printf("%s[%d]==0", array->name, array->idx);	
				if(sign==1)
					printf("%s[%d]>=0", array->name, array->idx);
				if(sign==-1)
					printf("%s[%d]<=0", array->name, array->idx);
			}
			break;
		case PLUS_TYPE: 
			{
				bin_t* bin = (bin_t*)expr;
				printf("(");
				if(sign==0){
					isPredictable(bin->a,0);
					printf("&&");
					isPredictable(bin->b,0);
				}
				if(sign==1){
					isPredictable(bin->a,1);
					printf("&&");
					isPredictable(bin->b,1);
				}	
				if(sign==-1){
					isPredictable(bin->a,-1);
					printf("&&");
					isPredictable(bin->b,-1);
				}	
				printf(")");
			}
			break;
		case MINUS_TYPE: 
			{
				bin_t* bin = (bin_t*)expr;
				printf("(");
				if(sign==0){
					isPredictable(bin->a,0);
					printf("&&");
					isPredictable(bin->b,0);
				}
				if(sign==1){
					isPredictable(bin->a,1);
					printf("&&");
					isPredictable(bin->b,-1);
				}	
				if(sign==-1){
					isPredictable(bin->a,-1);
					printf("&&");
					isPredictable(bin->b,1);
				}	
				printf(")");
			}
			break;
		case MULT_TYPE: 
			{
				bin_t* bin = (bin_t*)expr;
				printf("(");
				if(sign==0){
					isPredictable(bin->a,sign);
					printf("||");
					isPredictable(bin->b,sign);
				}
				if(sign==1){
					printf("(");
					isPredictable(bin->a,1);
					printf("&&");
					isPredictable(bin->b,1);
					printf(")||(");
					isPredictable(bin->a,-1);
					printf("&&");
					isPredictable(bin->b,-1);
					printf(")");
				}	
				if(sign==-1){
					printf("(");
					isPredictable(bin->a,1);
					printf("&&");
					isPredictable(bin->b,-1);
					printf(")||(");
					isPredictable(bin->a,-1);
					printf("&&");
					isPredictable(bin->b,1);
					printf(")");
				}	
				printf(")");
			}
			break;
		case NEG_TYPE: 
			{
				neg_t* neg = (neg_t*)expr;
				isPredictable(neg->expr,sign);
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
	while(reduceExpression(assign->expr,&assign->expr)){printf("Reduce....\n");}

	printPrototype(fname,args);

	//Normal epsilon calculation
	printf("\tdouble epserr=0;\n");
	printExprInexact(assign->expr,0,1);
	
	printf("\n\tif(partial0>epserr||partial0<-epserr) return partial0;\n");


	//Explicit zero
	printf("\tif(");
	isPredictable(assign->expr,0);
	printf(")\n\t{return 0;}\n");

	//Explicit one
	printf("\tif(");
	isPredictable(assign->expr,1);
	printf(")\n\t{return 1;}\n");

	//Explicit neg
	printf("\tif(");
	isPredictable(assign->expr,-1);
	printf(")\n\t{return -1;}\n");


	printf("\treturn nan;\n");

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


