#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

using namespace std;

#include "y.tab.h"
#include "ast.h"
#include "main.h"

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



	createssa(assign->expr);
	//Normal epsilon calculation
	createEpsPartials();

	printPrototype(fname,args);
	printPartials();

	printEpsPartials();

	int nparts = getNumPartials()-1;
	printf("\n\tif(partial%d>epserr||partial%d<-epserr) return partial%d;\n",nparts,nparts,nparts);


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


