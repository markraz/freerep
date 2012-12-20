#pragma once

vector<char*> getArgs(arg_t* args);
void addArg(char* name, vector<char*> *args);
void getUsedArgs(ast_t* expr, vector<char*> *args);
void checkUsedArgs(vector<char*> *usedargs, vector<char*> *args);
bool reduceExpression(ast_t* expr, ast_t** prt);
vector<ast_t*>* getStatements();
void createssa(ast_t* ast);
vector<ssa_t*> getSsaVec();
void printPartials();
void createEpsPartials();
void printEpsPartials();
int getNumPartials();
ast_t* copy(ast_t* ast);
