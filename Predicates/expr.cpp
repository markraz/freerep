#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

using namespace std;

#include "ast.h"
#include "main.h"

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
				  	number_t *num = (number_t*)number(res);
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
				  	number_t *num = (number_t*)number(res);
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
