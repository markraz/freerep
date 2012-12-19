#pragma once

vector<char*> getArgs(arg_t* args);
void addArg(char* name, vector<char*> *args);
void getUsedArgs(ast_t* expr, vector<char*> *args);
void checkUsedArgs(vector<char*> *usedargs, vector<char*> *args);
