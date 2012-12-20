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
		case SUM_TYPE: //fall through
		case MULT_TYPE:
			{
				bin_t* bin = (bin_t*)ast;
				printf("(");
				printEpsTree(bin->a);
				if(ast->type==MULT_TYPE)
					printf(" X ");
				if(ast->type==SUM_TYPE)
					printf(" +/- ");
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
		case NUMBER_TYPE:
			{
				number_t* num = (number_t*)ast;
				printf("%d", num->val);
			}
			break;
	}
}

ast_t* copy(ast_t* ast){
	switch(ast->type){
		case PART_TYPE:
			{
				part_t* part = (part_t*)ast;
				part_t* npart = (part_t*)malloc(sizeof(part_t));
				*npart = *part;
				return &npart->ast;
			}
		case MAG_TYPE:
			{
			 	mag_t* mag = (mag_t*)ast;
				mag_t* nmag = (mag_t*)malloc(sizeof(mag_t));
				*nmag = *mag;
				nmag->expr = copy(mag->expr);
				return &nmag->ast;
			}
		case EPS_TYPE:
			{
				eps_t* eps = (eps_t*)ast;
				eps_t* neps = (eps_t*)malloc(sizeof(eps_t));
				*neps = *eps;
				return &neps->ast;
			}
		case PLUS_TYPE:
		case MINUS_TYPE:
		case SUM_TYPE:
		case MULT_TYPE:
			{
				bin_t* bin = (bin_t*)ast;
				bin_t* nbin = (bin_t*)malloc(sizeof(bin_t));
				*nbin = *bin;
				nbin->a = copy(bin->a);
				nbin->b = copy(bin->b);
				return &nbin->ast;
			}
		case NUMBER_TYPE:
			{
				number_t* num = (number_t*)ast;
				number_t* nnum = (number_t*)malloc(sizeof(number_t));
				*nnum = *num;
				return &nnum->ast;
			}
		default:
			printf("Fail %d %s\n", ast->type, __func__);
	}
}

bool simplify(ast_t* ast, ast_t** past){
	if(ast->type == MULT_TYPE){
		bin_t* bin = (bin_t*)ast;
		if(bin->a->type==SUM_TYPE){
			bin_t* sum = (bin_t*)bin->a;
			bin_t* proda = (bin_t*)mult(sum->a,bin->b);
			bin_t* prodb = (bin_t*)mult(sum->b,copy(bin->b));
			bin_t* nsum = (bin_t*)make_sum(&proda->ast,&prodb->ast);
			*past = &nsum->ast;
//			printf("Try a\n");
			return true;
		}
		if(bin->b->type==SUM_TYPE){
			bin_t* sum = (bin_t*)bin->b;
			bin_t* proda = (bin_t*)mult(sum->a,bin->a);
			bin_t* prodb = (bin_t*)mult(sum->b,copy(bin->a));
			bin_t* nsum = (bin_t*)make_sum(&proda->ast,&prodb->ast);
			*past = &nsum->ast;
//			printf("Try b\n");
			return true;
		}
		return simplify(bin->a,&bin->a)||simplify(bin->b,&bin->b);
	}
	if(ast->type == SUM_TYPE){
		bin_t* bin = (bin_t*)ast;
		return simplify(bin->a,&bin->a)||simplify(bin->b,&bin->b);
	}
	return false;
}

void printEpsPartials(){
	for(int i=0; i < ssavec.size(); i++){
		ssa_t* ssa = ssavec.at(i);
		if(ssa->ast->type==NEG_TYPE){
			//no error
		}else{ //plus minus mult
			bin_t* bin = (bin_t*)ssa->ast;
			if(isErrFree(bin->a) && isErrFree(bin->b)){
				part_t* parta = (part_t*)make_part(i);
				part_t* partb = (part_t*)make_part(i);
 				//mag_t* mag = (mag_t*)make_mag(&partb->ast);
				eps_t* eps = (eps_t*)make_eps();
				bin_t* nbin = (bin_t*)mult(&partb->ast,&eps->ast);	
				bin_t* asum = (bin_t*)make_sum(&parta->ast,&nbin->ast);
				ssa->eps = &asum->ast;

				printf("\tconst char* epspart%d=\"",i);
				printEpsTree(&asum->ast);
				printf("\";\n");
//				printf("\tconst char* epspart%d=\"partial%d +/- (mag(partial%d) * e)\";\n",i,i,i);
			}else{
				if(bin->ast.type != MULT_TYPE){
					printf("freak out\n");
					continue;
				}
				//Multiply the 2 eps trees
				ast_t* prod = mult(bin->a->ssa->eps, bin->b->ssa->eps);
/*				printf("\tconst char* epspart%d=\"",i);
				printEpsTree(prod);
				printf("\";\n");*/
				while(simplify(prod,&prod)){
/*					printf("Reduce....\n");	
					printf("\tconst char* epspart%d=\"",i);	
					printEpsTree(prod);
					printf("\";\n"); */
				}

				ntree_t* tree = toNTree(prod);
				while(orderFactors(tree)){
//					printf("Reduce....\n");	
				}
				while(combineTerms(tree)){
//					printf("Reduce...\n");
				}
				part_t* part = (part_t*)make_part(i);
				eps_t* eps = (eps_t*)make_eps();
				ast_t* nsum = 	make_sum(&part->ast,&eps->ast);

				factorAndReplace(tree,make_part(bin->a->ssa->idx),make_part(bin->b->ssa->idx),nsum);
				prod = fromNTree(tree);

//				while(simplify(prod,&prod)){
//					printf("Reduce....\n");	
//					} 


				printf("\tconst char* epspart%d=\"",i);
				printEpsTree(prod);
				printf("\";\n");
		
			}
		}
	}
}


