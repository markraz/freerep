#include <stdio.h>
#include <string.h>
#include "y.tab.h"
#include "ast.h"

extern "C"{
 
void yyerror(const char *str)
{
        fprintf(stderr,"error: %s\n",str);
}
 
int yywrap()
{
        return 1;
} 

} //Extern "C"
  
main()
{
        yyparse();
} 


