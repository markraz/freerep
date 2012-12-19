#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

using namespace std;

#include "ast.h"
#include "main.h"

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
