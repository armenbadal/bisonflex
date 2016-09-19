
#include <gc.h>

#include "ast.h"

/**/
void expression_as_lisp( struct expression* expr, FILE* out )
{
  switch( expr->kind ) {
    case NUMBER:
	  fprintf(out, "%lf", expr->number);
	  break;
    case VARIABLE:
	  fprintf(out, "%s", expr->ident);
	  break;
    case UNRY:
	  fprintf(out, "(");
	  if( NEG == expr->oper )
		fprintf(out,"-");
	  else if( NOT == expr->oper )
		fprintf(out,"not");
	  expression_as_lisp(expr->exo, out);
	  fprintf(out, ")");
	  break;
    case BINARY:
	  break;
    case APPLY:
	  break;
  }
}

