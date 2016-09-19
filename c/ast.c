
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
    case UNARY:
	  fprintf(out, "(");
	  if( NEG == expr->oper )
		fprintf(out,"- ");
	  else if( NOT == expr->oper )
		fprintf(out,"not ");
	  expression_as_lisp(expr->exo, out);
	  fprintf(out, ")");
	  break;
    case BINARY:
	  fprintf(out, "(");
	  switch( expr->oper ) {
	    case OR:
		  fprintf(out, "or");
		  break;
	    case AND:
		  fprintf(out, "and");
		  break;
	    case EQ:
		  fprintf(out, "=");
		  break;
	    case NE:
		  fprintf(out, "/=");
		  break;
	    case GT:
		  fprintf(out, ">");
		  break;
	    case GE:
		  fprintf(out, ">=");
		  break;
	    case LT:
		  fprintf(out, "<");
		  break;
	    case LE:
		  fprintf(out, "<=");
		  break;
	    case ADD:
		  fprintf(out, "+");
		  break;
	    case SUB:
		  fprintf(out, "-");
		  break;
	    case MUL:
		  fprintf(out, "*");
		  break;
	    case DIV:
		  fprintf(out, "/");
		  break;
	    case POW:
		  fprintf(out, "expt");
		  break;
	  }
	  fprintf(out, " ");
	  expression_as_lisp(expr->exo, out);
	  fprintf(out, " ");
	  expression_as_lisp(expr->exi, out);
	  fprintf(out, ")");
	  break;
    case APPLY:
	  // TODO իրականացնել ֆունկցիայի կիրառության թարգմանությունը
	  break;
  }
}


/**/
void statement_as_lisp( struct statement* stat, FILE* out )
{
  switch( stat->kind ) {
    case INPUT: {
	  struct input_s* inp = (struct input_s*)stat->child;
	  fprintf(out, "(read %s)", inp->vari);
	  break;
	}
    case PRINT: {
	  struct print_s* prp = (struct print_s*)stat->child;
	  fprintf(out, "(print ");
	  expression_as_lisp(prp->valu, out);
	  fprintf(out, ")");
	  break;
	}
    case ASSIGN: {
	  struct assignment_s* asp = (struct assignment_s*)stat->child;
	  fprintf(out, "(setf %s ", asp->vari);
	  expression_as_lisp(asp->valu, out);
	  fprintf(out, ")");
	  break;
	}
    case IF: {
	  struct if_s* brp = (struct if_s*)stat->child;
	  fprintf(out, "(if ");
	  expression_as_lisp(brp->cond, out);
	  fprintf(out, " ");
	  statement_as_lisp(brp->thenp, out);
	  fprintf(out, " ");
	  statement_as_lisp(brp->elsep, out);
	  fprintf(out, ")");
	  break;
	}
    case FOR: {
	  struct for_s* fop = (struct for_s*)stat->child;
	  // param, start, stop, step, body
	  break;
	}
    case WHILE: {
	  struct while_s* whp = (struct while_s*)stat->child;
	  // cond, body
	  break;
	}
    case CALL: {
	  struct call_s* cap = (struct call_s*)stat->child;
	  // func, argus
	  break;
	}
    case SEQ: {
	  struct sequence_s* sep = (struct sequence_s*)stat->child;
	  fprintf(out, "(progn ");
	  struct node* sp = (struct node*)sep->elems;
	  while( sp != NULL ) {
		statement_as_lisp((struct statement*)sp->data, out);
		sp = sp->next;
	  }
	  fprintf(out, ")");
	  break;
	}
  }
}


