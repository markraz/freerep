#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

using namespace std;

#include "ast.h"
#include "main.h"
#include "ntree.h"

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
					printf(" * ");
				if(ast->type==SUM_TYPE)
					printf(" + ");
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

void createEpsPartials(){
	vector<ssa_t*> ssavec = getSsaVec();
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

/*				printf("\tconst char* epspart%d=\"",i);
				printEpsTree(&asum->ast);
				printf("\";\n"); */
//				printf("\tconst char* epspart%d=\"partial%d +/- (mag(partial%d) * e)\";\n",i,i,i);
			}else{
				if(bin->ast.type == MINUS_TYPE){
					part_t* parta = (part_t*)make_part(i);
					part_t* partb = (part_t*)make_part(i);
					eps_t* eps = (eps_t*)make_eps();
					bin_t* nbin = (bin_t*)mult(&partb->ast,&eps->ast);	
					bin_t* asum = (bin_t*)make_sum(&parta->ast,&nbin->ast);
					part_t* partc = (part_t*)make_part(bin->a->ssa->idx);
					part_t* partd = (part_t*)make_part(bin->b->ssa->idx);
					eps_t* epartb = (eps_t*)make_eps();
					eps_t* epartc = (eps_t*)make_eps();
					bin_t* abin = (bin_t*)mult(&epartb->ast,&partc->ast);	
					bin_t* bbin = (bin_t*)mult(&epartc->ast,&partd->ast);	
					bin_t* bsum = (bin_t*)make_sum(&asum->ast,&abin->ast);
					bin_t* csum = (bin_t*)make_sum(&bsum->ast,&bbin->ast);
					//TODO: a bunch of other crap
					ssa->eps = &csum->ast;
/*					printf("\tconst char* epspart%d=\"",i);	
					printEpsTree(&asum->ast);
					printf("\";\n"); */
					continue;
				}
				if(bin->ast.type == MULT_TYPE){
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
					part_t* parta = (part_t*)make_part(i);
					part_t* partb = (part_t*)make_part(i);
					eps_t* eps = (eps_t*)make_eps();
					bin_t* nbin = (bin_t*)mult(&partb->ast,&eps->ast);	
					ast_t* nsum = 	make_sum(&parta->ast,&nbin->ast);
	
					factorAndReplace(tree,make_part(bin->a->ssa->idx),make_part(bin->b->ssa->idx),nsum);
					prod = fromNTree(tree);
	
					while(simplify(prod,&prod)){
	//					printf("Reduce....\n");	
					} 
	
					tree = toNTree(prod);
					while(orderFactors(tree)){
	//					printf("Reduce....\n");	
					}
					while(combineTerms(tree)){
	//					printf("Reduce...\n");
					}
					prod = fromNTree(tree);
	
					ssa->eps = prod;
	
	/*				printf("\tconst char* epspart%d=\"",i);
					printEpsTree(prod);
					printf("\";\n"); */
				}	
			}
		}
	}
}

void printEpsDefine(int idx){
	vector<ssa_t*> ssavec = getSsaVec();
	ssa_t* ssa = ssavec.at(idx);

	ntree_t* tree = toNTree(copy(ssa->eps));
	while(epsOnly(tree)){}
	factor(tree,make_part(idx));
	ast_t* ast = fromNTree(tree);
	
	printf("#define epsfact%d ", idx);
	printEpsTree(ast);
	printf("\n");
}

void printEpsPartial(int idx){
	vector<ast_t*> factors;
	vector<ssa_t*> ssavec = getSsaVec();
	ssa_t* ssa = ssavec.at(idx);	

	ntree_t* tree = toNTree(copy(ssa->eps));
	while(epsOnly(tree)){}
	getFactors(tree,&factors);
	ast_t* ast = fromNTree(tree);
	//Only need a list of partials involved

	for(int i=0; i < factors.size(); i++){
		part_t* part = (part_t*)factors.at(i);
		printEpsDefine(part->idx);
	}	
/*	for(int i=0; i < factors.size(); i++){
		part_t* part = (part_t*)factors.at(i);
		if(idx!=part->idx)
			printEpsPartial(part->idx);
	}*/

	printf("\tdouble epserr=");
	for(int i=0; i < factors.size(); i++){
		part_t* part = (part_t*)factors.at(i);
		printf("ABS(partial%d)*epsfact%d",part->idx,part->idx);
		if(i < factors.size()-1)
			printf("+");
	}
	printf(";\n");
}

void printEpsPartials(){
	vector<ssa_t*> ssavec = getSsaVec();
	printEpsPartial(ssavec.size() - 1);
}
