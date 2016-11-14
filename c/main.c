
#include <stdio.h>

#include <gc.h>

#include "ast.h"

program* prog = NULL;

/**/
int main()
{
  prog = GC_MALLOC(sizeof(program));

  extern int yyparse();
  int ok = yyparse();
  if( 0 == ok )
	  	program_as_json(prog, stdout);

  return ok;
}

