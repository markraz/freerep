#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

using namespace std;

#include "ast.h"
#include "main.h"
#include "ntree.h"

void findSums(ast_t* ast, vector<ast_t*> *sums){
	if(ast->type == SUM_TYPE){
		bin_t *bin = (bin_t*)ast;
		findSums(bin->a,sums);
		findSums(bin->b,sums);
	}else{
		sums->push_back(ast);
	}
}

void findProds(ast_t* ast, vector<ast_t*> *prods){
	if(ast->type == MULT_TYPE){
		bin_t *bin = (bin_t*)ast;
		findProds(bin->a,prods);
		findProds(bin->b,prods);
	}else{
		prods->push_back(ast);
	}
}

ntree_t* toProdTree(ast_t* ast){
	vector<ast_t*> prods;
	findProds(ast,&prods);

	ntree_t *prodtree = (ntree_t*)malloc(sizeof(ntree_t));		
	prodtree->type = MULT_TYPE;
	prodtree->size = prods.size();
	prodtree->asts = (ast_t**)malloc(prodtree->size*sizeof(void*));
	prodtree->trees = 0;
	for(int i=0; i < prodtree->size; i++){
		prodtree->asts[i] = prods.at(i);
	}
	return prodtree;
}

ntree_t* toNTree(ast_t* ast){
	vector<ast_t*> sums;
	findSums(ast,&sums);

	ntree_t *sumtree = (ntree_t*)malloc(sizeof(ntree_t));
	sumtree->type = SUM_TYPE;
	sumtree->size = sums.size();
	sumtree->asts = (ast_t**)malloc(sumtree->size*sizeof(void*));
	sumtree->trees = (ntree_t**)malloc(sumtree->size*sizeof(void*));
	for(int i=0; i < sumtree->size; i++){
		sumtree->asts[i] = sums.at(i);
		sumtree->trees[i] = toProdTree(sumtree->asts[i]);
	}


	return sumtree;
}

ast_t* fromProdTree(ntree_t* tree){
	ast_t* ret=0;
	bin_t* prods = (bin_t*)malloc((tree->size-1)*sizeof(bin_t));
	if(tree->size==1){
		return tree->asts[0];
	}
	for(int i=0; i < tree->size-1; i++){
		prods[i].ast.type = MULT_TYPE;
		prods[i].a = tree->asts[i];
		prods[i].b = &prods[i + 1].ast;
	}
	prods[tree->size-2].b = tree->asts[tree->size-1];
	return (ast_t*)prods;
}	

ast_t* fromNTree(ntree_t* tree){
	ast_t* ret=0;
	bin_t* sums = (bin_t*)malloc((tree->size-1)*sizeof(bin_t));
	if(tree->size-1 == 0){
//		printf("Short tree\n");
		return fromProdTree(tree->trees[0]);
	}
	for(int i=0; i < tree->size-1; i++){
		sums[i].ast.type = SUM_TYPE;
		sums[i].a = fromProdTree(tree->trees[i]);
		sums[i].b = &sums[i + 1].ast;
	}
	sums[tree->size-2].b = fromProdTree(tree->trees[tree->size-1]);
	return (ast_t*)sums;
}

ast_t* getTerminal(ast_t* ast){
	switch(ast->type){
		case EPS_TYPE:
		case NUMBER_TYPE:
		case PART_TYPE: return ast;
		default: printf("Fail %d %s\n", ast->type, __func__);
	}
	return 0;
}

bool orderFactorsProd(ntree_t* tree){
	for(int i=0; i < tree->size; i++){
		for(int j=i+1; j < tree->size; j++){
			ast_t* a = getTerminal(tree->asts[i]);
			ast_t* b = getTerminal(tree->asts[j]);
			part_t* pa = (part_t*)a;
			part_t* pb = (part_t*)b;
			if((a->type == EPS_TYPE && b->type != EPS_TYPE) || b->type == NUMBER_TYPE || (
				b->type == PART_TYPE && a->type == PART_TYPE && (pb->idx < pa->idx))){
				tree->asts[j] = a;
				tree->asts[i] = b;
				return true;
			} 
		}
	}
	return false;
}

bool orderFactors(ntree_t* tree){
	for(int i=0; i < tree->size; i++){
		if(orderFactorsProd(tree->trees[i]))
			return true;
	}
	return false;
}

void insertProd(ntree_t* tree, ast_t* factor){
	ast_t** asts = (ast_t**)malloc((tree->size+1)*sizeof(void*));
	asts[0] = factor;
	for(int i=0; i < tree->size; i++){
		asts[i+1] = tree->asts[i];
	}
	
	tree->size++;
	tree->asts = asts;
}

void removeSum(ntree_t* tree, ntree_t* prod){
	int j=0;
	for(int i=0; i < tree->size-1; i++){
		if(tree->trees[i] == prod)
			j++;
		tree->trees[i] = tree->trees[j];
		tree->asts[i] = tree->asts[j];
		j++;
	}
	tree->size--;
}

bool combineTerms(ntree_t* tree){
	for(int i=0; i < tree->size; i++){
		for(int j=i+1; j < tree->size; j++){
			ntree_t* a = tree->trees[i];
			ntree_t* b = tree->trees[j];
//			if(a->size != b->size)
//				continue;
			bool error = false;
			int ka=0;
			int kb=0;
			for(; ka < a->size && kb < b->size;){
				ast_t* ta = getTerminal(a->asts[ka]);
				ast_t* tb = getTerminal(b->asts[kb]);
				if(ta->type==NUMBER_TYPE){
					ka++;
//					printf("ka\n");
					continue;
				}
				if(tb->type==NUMBER_TYPE){
					kb++;
//					printf("kb\n");
					continue;
				}
				ka++; kb++;
				if(ta->type != tb->type){
					error=true; break;
				}
				if(ta->type != PART_TYPE)
					continue;
				part_t* pa = (part_t*)ta;
				part_t* pb = (part_t*)tb;
				if(pa->idx != pb->idx){
					error=true; break;
				}
			}
//			printf("%d %d %d %d %d\n", error,ka,a->size,kb,b->size);
			if(error==false && ka==a->size && kb==b->size){
				ast_t* ta = getTerminal(a->asts[0]);
				ast_t* tb = getTerminal(b->asts[0]);

				number_t *num = (number_t*)number(1);
				if(ta->type==NUMBER_TYPE)
					num = (number_t*)ta;
				if(tb->type==NUMBER_TYPE){
					num->val += ((number_t*)tb)->val;
				}else{
					num->val++;
				}						
//				printf("Found one\n");
				if(ta->type!=NUMBER_TYPE)
					insertProd(a,(ast_t*)num);
				removeSum(tree,b);
				return true;
			}
		}
	}
	return false;
}

bool equiv(ast_t* a, ast_t* b){
	if(b->type != a->type){
	//	printf("%d %d\n", a->type, b->type);
		return false;
	}
	switch(a->type){
		case PART_TYPE:
			{
				part_t *pa = (part_t*)a;
				part_t *pb = (part_t*)b;
	//			printf("%d %d\n", pa->idx, pb->idx);
				return pa->idx == pb->idx;
			}
		default:
			printf("Fail %d %s\n", a->type, __func__);
	}
}

void removeProd(ntree_t* tree, ast_t* prod){
	int j=0;
//	printf("%d\n", tree->size);
	for(int i=0; i < tree->size-1; i++){
		if(equiv(tree->asts[i],prod))
			j++;
		tree->asts[i] = tree->asts[j];
		j++;
	}
	tree->size--;
//	printf("%d\n", tree->size);
}


void factorAndReplace(ntree_t* tree, ast_t* a, ast_t* b,ast_t* rep){
	for(int i=0; i < tree->size; i++){
		ntree_t* ptree = tree->trees[i];
		int afound=-1;
		int bfound=-1;
	//	printf("%d %d\n", tree->size, ptree->size);
		for(int j=0; j < ptree->size; j++){
			if(equiv(ptree->asts[j],a))
				afound=j;
			if(equiv(ptree->asts[j],b))
				bfound=j;
		}
		if(afound==-1 || bfound==-1)
			continue;

		removeProd(ptree,a);
		removeProd(ptree,b);
		insertProd(ptree,copy(rep));
	}
}

bool epsOnly(ntree_t* tree){
	for(int i=0; i < tree->size; i++){
		ntree_t* ptree = tree->trees[i];
		bool found=false;
		for(int j=0; j < ptree->size; j++){
			if(getTerminal(ptree->asts[j])->type == EPS_TYPE){
				found=true;
				break;
			}
		}
		if(!found){
			removeSum(tree,ptree);
			return true;
		}
	}	
	return false;
}

void getFactors(ntree_t* tree,vector<ast_t*>*factors){
	for(int i=0; i < tree->size; i++){
		ntree_t* ptree = tree->trees[i];
		for(int j=0; j < ptree->size; j++){
			if(ptree->asts[j]->type!=PART_TYPE)
				continue;
			bool found=false;
			for(int k=0; k < factors->size(); k++){
				if(((part_t*)factors->at(k))->idx == ((part_t*)ptree->asts[j])->idx){
					found=true;
					break;
				}
			}
			if(found)
				continue;
			factors->push_back(ptree->asts[j]);
		}
	}
}

void factor(ntree_t* tree, ast_t* fac){
	if(fac->type!=PART_TYPE)
		printf("Fail\n");

	vector<ntree_t*> tokill;
	for(int i=0; i < tree->size; i++){
		ntree_t* ptree = tree->trees[i];
		bool found=false;
		for(int j=0; j < ptree->size; j++){
			if(ptree->asts[j]->type!=PART_TYPE)
				continue;
//			printf("%d %d\n", ((part_t*)ptree->asts[j])->idx, ((part_t*)fac)->idx);
			if(equiv(ptree->asts[j],fac)){
//				printf("found\n");
				found=true;
				break;
			}
		}
		if(found){
			removeProd(ptree,fac);
		}else{
			tokill.push_back(ptree);
		}		
	}	

	for(int i=0; i < tokill.size(); i++){
		removeSum(tree,tokill.at(i));
	}
}
