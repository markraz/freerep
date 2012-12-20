#pragma once

struct ntree;
typedef struct ntree{
	int type;
	int size;
	struct ntree** trees;
	ast_t** asts;
} ntree_t;

ntree_t* toNTree(ast_t*);
ast_t* fromNTree(ntree_t* tree);
bool orderFactors(ntree_t* tree);
bool combineTerms(ntree_t* tree);
bool epsOnly(ntree_t* tree);
void getFactors(ntree_t* tree,vector<ast_t*>*factors);
void factorAndReplace(ntree_t* tree, ast_t* a, ast_t* b,ast_t* nsum);
void factor(ntree_t* tree, ast_t* fac);
