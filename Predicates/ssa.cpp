#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

using namespace std;

#include "ast.h"
#include "main.h"
#include "ntree.h"

static vector<ssa_t*> ssavec;

int getNumPartials(){
	return ssavec.size();
}

vector<ssa_t*> getSsaVec(){
	return ssavec;
}

void createssa(ast_t* ast){
	switch(ast->type){
		case NUMBER_TYPE: break;
		case ARRAY_TYPE:  break;
		case PLUS_TYPE: //fall through
		case MINUS_TYPE: //fall through
		case MULT_TYPE: 
			{
				bin_t* bin = (bin_t*)ast;
				createssa(bin->a);
				createssa(bin->b);
				ssa_t* ssa = (ssa_t*)malloc(sizeof(ssa_t));
				bin->ast.ssa = ssa;
				ssa->ast = ast;
				ssa->idx = ssavec.size();	
				ssa->eps = 0;	
				ssavec.push_back(ssa);
			}
			break;
		case NEG_TYPE: 
			{
				neg_t* neg = (neg_t*)ast;
				printf("ENOIMPL-");
				createssa(neg->expr);
	
				ssa_t* ssa = (ssa_t*)malloc(sizeof(ssa_t));
				neg->ast.ssa = ssa;
				ssa->ast = ast;
				ssa->idx = ssavec.size();	
				ssa->eps = 0;	
				ssavec.push_back(ssa);
			}
			break;
		default: printf("Fail %d\n", ast->type); 
	}
}

void printPartialSrc(ast_t* expr){
//	printf("%p\n",expr);
	switch(expr->type){
		case NUMBER_TYPE: 
			{
				number_t* number = (number_t*)expr;
				printf("%d",number->val);
			}				
			break;
		case ARRAY_TYPE: 
			{
				array_t* array = (array_t*)expr;
				printf("%s[%d]", array->name, array->idx);	
			}
			break;
		case PLUS_TYPE: //fall through
		case MINUS_TYPE: //fall through
		case MULT_TYPE: //fall through
		case NEG_TYPE:
			{
				printf("partial%d",expr->ssa->idx);
			}
			break;
	}
}

void printPartials(){
	for(int i=0; i < ssavec.size(); i++){
		printf("\tdouble partial%d=",i);
		ssa_t* ssa = ssavec.at(i);
		if(ssa->ast->type==NEG_TYPE){
			neg_t* neg = (neg_t*)ssa->ast;
			printPartialSrc(neg->expr);
		}else{ //plus minus mult
			bin_t* bin = (bin_t*)ssa->ast;
			printPartialSrc(bin->a);
			if(ssa->ast->type==MULT_TYPE)
				printf("*");
			if(ssa->ast->type==MINUS_TYPE)
				printf("-");
			if(ssa->ast->type==PLUS_TYPE)
				printf("+");
			printPartialSrc(bin->b);
		}
		printf(";\n");
	}
}


