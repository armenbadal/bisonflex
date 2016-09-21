
#include <string.h>

#include <gc.h>

#include "ast.h"

/**/
expression* create_number( double val )
{
  expression* expr = GC_MALLOC(sizeof(expression));
  expr->kind = NUMBER;
  expr->number = val;
  return expr;
}

/**/
expression* create_variable( const char* nm )
{
  expression* expr = GC_MALLOC(sizeof(expression));
  expr->kind = VARIABLE;
  expr->name = GC_MALLOC(1 + strlen(nm));
  strcpy(expr->name, nm);
  return expr;
}

/**/
expression* create_unary( int op, expression* eo )
{
  expression* expr = GC_MALLOC(sizeof(expression));
  expr->kind = UNARY;
  expr->oper = op;
  expr->exo = eo;
  return expr;
}

/**/
expression* create_binary( int op, expression* eo, expression* ei )
{
  expression* expr = GC_MALLOC(sizeof(expression));
  expr->kind = BINARY;
  expr->oper = op;
  expr->exo = eo;
  expr->exi = ei;
  return expr;
}

/**/
expression* create_apply( function* fu, node* ps )
{
  expression* expr = GC_MALLOC(sizeof(expression));
  expr->kind = APPLY;
  expr->func = fu;
  expr->exs = ps;
  return expr;
}

/**/
void expression_as_lisp( expression* expr, FILE* out )
{
  switch( expr->kind ) {
    case NUMBER:
	  fprintf(out, "%lf", expr->number);
	  break;
    case VARIABLE:
	  fprintf(out, "%s", expr->name);
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
static statement* create_statement( int sk, void* chd )
{
  statement* stat = GC_MALLOC(sizeof(statement));
  stat->kind = sk; stat->child = chd;
  return stat;
}
  
/**/
statement* create_input( const char* vr )
{
  input_s* inp = GC_MALLOC(sizeof(input_s));
  inp->vari = GC_MALLOC(1 + strlen(vr));
  strcpy(inp->vari, vr);
  return create_statement(INPUT, inp);
}

/**/
statement* create_print( expression* ex )
{
  print_s* pri = GC_MALLOC(sizeof(print_s));
  pri->valu = ex;
  return create_statement(PRINT, pri);
}

/**/
statement* create_assign( const char* vr, expression* vl )
{
  assign_s* asi = GC_MALLOC(sizeof(assign_s));
  asi->vari = GC_MALLOC(1 + strlen(vr));
  strcpy(asi->vari, vr);
  asi->valu = vl;
  return create_statement(ASSIGN, asi);
}

/**/
statement* create_if( expression* co, statement* tp, statement* ep )
{
  if_s* ifp = GC_MALLOC(sizeof(if_s));
  ifp->cond = co;
  ifp->thenp = tp;
  ifp->elsep = ep;
  return create_statement(IF, ifp);
}

/**/
statement* create_for( const char* pr, expression* sa, expression* se, expression* sp, statement* bo )
{
  for_s* fop = GC_MALLOC(sizeof(for_s));
  fop->param = GC_MALLOC(1 + strlen(pr));
  strcpy(fop->param, pr);
  fop->start = sa;
  fop->stop = se;
  fop->step = sp;
  fop->body = bo;
  return create_statement(FOR, fop);
}

/**/
statement* create_while( expression* co, statement* bo )
{
  while_s* whi = GC_MALLOC(sizeof(while_s));
  whi->cond = co;
  whi->body = bo;
  return create_statement(WHILE, whi);
}

/**/
statement* create_call( function* fu, node* ag )
{
  call_s* cap = GC_MALLOC(sizeof(call_s));
  cap->func = fu;
  cap->argus = ag;
  return create_statement(CALL, cap);
}

/**/
statement* create_sequence( node* el )
{
  sequence_s* sep = GC_MALLOC(sizeof(sequence_s));
  sep->elems = el;
  return create_statement(SEQ, sep);
}

/**/
void statement_as_lisp( statement* stat, FILE* out )
{
  switch( stat->kind ) {
    case INPUT: {
	  input_s* inp = (input_s*)stat->child;
	  fprintf(out, "(read %s)", inp->vari);
	  break;
	}
    case PRINT: {
	  print_s* prp = (print_s*)stat->child;
	  fprintf(out, "(print ");
	  expression_as_lisp(prp->valu, out);
	  fprintf(out, ")");
	  break;
	}
    case ASSIGN: {
	  assign_s* asp = (assign_s*)stat->child;
	  fprintf(out, "(setf %s ", asp->vari);
	  expression_as_lisp(asp->valu, out);
	  fprintf(out, ")");
	  break;
	}
    case IF: {
	  if_s* brp = (if_s*)stat->child;
	  fprintf(out, "(if ");
	  expression_as_lisp(brp->cond, out);
	  fprintf(out, " ");
	  statement_as_lisp(brp->thenp, out);
	  if( brp->elsep != NULL ) {
		fprintf(out, " ");
		statement_as_lisp(brp->elsep, out);
	  }
	  fprintf(out, ")");
	  break;
	}
    case FOR: {
	  for_s* fop = (for_s*)stat->child;
	  // param, start, stop, step, body
	  break;
	}
    case WHILE: {
	  while_s* whp = (while_s*)stat->child;
	  // cond, body
	  break;
	}
    case CALL: {
	  call_s* cap = (call_s*)stat->child;
	  // func, argus
	  break;
	}
    case SEQ: {
	  sequence_s* sep = (sequence_s*)stat->child;
	  fprintf(out, "(progn ");
	  node* sp = (node*)sep->elems;
	  while( sp != NULL ) {
		statement_as_lisp((statement*)sp->data, out);
		sp = sp->next;
	  }
	  fprintf(out, ")");
	  break;
	}
  }
}

/**/
void function_as_lisp( function* stat, FILE* out )
{
  // TODO
}

/**/
void program_as_lisp( program* stat, FILE* out )
{
  // TODO
}

