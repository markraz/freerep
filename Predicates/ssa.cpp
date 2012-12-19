#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

using namespace std;

#include "ast.h"
#include "main.h"

static vector<ssa_t*> ssavec;

int getNumPartials(){
	return ssavec.size();
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

void printEpsPartialsOld(){
	for(int i=0; i < ssavec.size(); i++){
		ssa_t* ssa = ssavec.at(i);
		if(ssa->ast->type==NEG_TYPE){
			//no error
		}else{ //plus minus mult
			printf("\tepserr+=(ABS(");
			bin_t* bin = (bin_t*)ssa->ast;
			printPartialSrc(bin->a);
			printf(")+ABS(");
			printPartialSrc(bin->b);
			printf("))*EPS_DBL\n");
		}
	}
}

bool isErrFree(ast_t* ast){
	if(ast->type == NUMBER_TYPE || ast->type == ARRAY_TYPE)
		return true;
	return false;
}

void printEpsTree(ast_t* ast){
//	printf("%p\n", ast);
	switch(ast->type){
		case PART_TYPE:
			{
				part_t *part = (part_t*)ast;
				printf("partial%d", part->idx);
			}
			break;
		case SUM_TYPE:
			{
				sum_t* sum = (sum_t*)ast;
				printEpsTree(sum->expr);
				if(sum->next){
					printf(" +/- ");
					printEpsTree(sum->next);
				}
			}
			break;
		case MULT_TYPE:
			{
				bin_t* bin = (bin_t*)ast;
				printf("(");
				printEpsTree(bin->a);
				printf("*");
				printEpsTree(bin->b);
				printf(")");
			}
			break;
		case MAG_TYPE:
			{
				mag_t* mag = (mag_t*)ast;
				printf("|");
				printEpsTree(mag->expr);
				printf("|");
			}
			break;
		case EPS_TYPE:
			{
				printf("e");
			}
			break;
	}
}

void printEpsPartials(){
	for(int i=0; i < ssavec.size(); i++){
		ssa_t* ssa = ssavec.at(i);
		if(ssa->ast->type==NEG_TYPE){
			//no error
		}else{ //plus minus mult
			bin_t* bin = (bin_t*)ssa->ast;
			if(isErrFree(bin->a) && isErrFree(bin->b)){
				sum_t* asum = (sum_t*)malloc(sizeof(sum_t));
				sum_t* bsum = (sum_t*)malloc(sizeof(sum_t));
				asum->ast.type = SUM_TYPE;
				asum->ast.ssa = 0;
				asum->next = &bsum->ast;
				bsum->ast.type = SUM_TYPE;
				bsum->ast.ssa = 0;
				bsum->next = 0;
				
				part_t* parta = (part_t*)malloc(sizeof(part_t));
				parta->ast.type = PART_TYPE;
				parta->ast.ssa = 0;
				parta->idx = i;
				asum->expr = &parta->ast;

				part_t* partb = (part_t*)malloc(sizeof(part_t));
				partb->ast.type = PART_TYPE;
				partb->ast.ssa = 0;
				partb->idx = i;

				mag_t* mag = (mag_t*)malloc(sizeof(mag_t));
				mag->ast.type = MAG_TYPE;
				mag->ast.ssa = 0;
				mag->expr = &partb->ast;

				eps_t* eps = (eps_t*)malloc(sizeof(eps_t));
				eps->ast.type = EPS_TYPE;
				eps->ast.ssa = 0;	

				bin_t* nbin = (bin_t*)malloc(sizeof(bin_t));
				nbin->ast.type = MULT_TYPE;
				nbin->ast.ssa = 0;
				nbin->a = &mag->ast;
				nbin->b = &eps->ast;	
				bsum->expr = &nbin->ast;		
		
				ssa->eps = &asum->ast;

				printf("\tconst char* epspart%d=\"",i);
				printEpsTree(&asum->ast);
				printf("\";\n");
//				printf("\tconst char* epspart%d=\"partial%d +/- (mag(partial%d) * e)\";\n",i,i,i);
			}
		}
	}
}


