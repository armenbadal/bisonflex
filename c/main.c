
#include <stdio.h>

#include <gc.h>

#include "ast.h"
#include "slist.h"

program* prog = NULL;

/**/
int main()
{
  prog = GC_MALLOC(sizeof(program));

  extern int yyparse();
  int ok = yyparse();
  return ok;
}

