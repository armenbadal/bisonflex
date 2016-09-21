
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
  if( expr == NULL )
	return;
  
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
		fprintf(out, "basic-neg ");
	  else if( NOT == expr->oper )
		fprintf(out, "basic-not ");
	  expression_as_lisp(expr->exo, out);
	  fprintf(out, ")");
	  break;
    case BINARY:
	  fprintf(out, "(");
	  switch( expr->oper ) {
	    case OR:
		  fprintf(out, "basic-or");
		  break;
	    case AND:
		  fprintf(out, "basic-and");
		  break;
	    case EQ:
		  fprintf(out, "basic-eq");
		  break;
	    case NE:
		  fprintf(out, "basic-ne");
		  break;
	    case GT:
		  fprintf(out, "basic-gt");
		  break;
	    case GE:
		  fprintf(out, "basic-ge");
		  break;
	    case LT:
		  fprintf(out, "basic-lt");
		  break;
	    case LE:
		  fprintf(out, "basic-le");
		  break;
	    case ADD:
		  fprintf(out, "basic-add");
		  break;
	    case SUB:
		  fprintf(out, "basic-sub");
		  break;
	    case MUL:
		  fprintf(out, "basic-mul");
		  break;
	    case DIV:
		  fprintf(out, "basic-div");
		  break;
	    case POW:
		  fprintf(out, "basic-pow");
		  break;
	  }
	  fprintf(out, " ");
	  expression_as_lisp(expr->exo, out);
	  fprintf(out, " ");
	  expression_as_lisp(expr->exi, out);
	  fprintf(out, ")");
	  break;
    case APPLY: {
	  fprintf(out, "(basic-apply %s ", expr->func->name);
	  node* ip = expr->exs;
	  while( ip != NULL ) {
		expression_as_lisp((expression*)(ip->data), out);
		ip = ip->next;
	  }
	  fprintf(out, ")");
	  break;
	}
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
  if( stat == NULL )
	return;
  
  switch( stat->kind ) {
    case INPUT: {
	  input_s* inp = (input_s*)stat->child;
	  fprintf(out, "(basic-input %s) ", inp->vari);
	  break;
	}
    case PRINT: {
	  print_s* prp = (print_s*)stat->child;
	  fprintf(out, "(basic-print ");
	  expression_as_lisp(prp->valu, out);
	  fprintf(out, ") ");
	  break;
	}
    case ASSIGN: {
	  assign_s* asp = (assign_s*)stat->child;
	  fprintf(out, "(basic-assign %s ", asp->vari);
	  expression_as_lisp(asp->valu, out);
	  fprintf(out, ") ");
	  break;
	}
    case IF: {
	  if_s* brp = (if_s*)stat->child;
	  fprintf(out, "(basic-if ");
	  expression_as_lisp(brp->cond, out);
	  fprintf(out, " ");
	  statement_as_lisp(brp->thenp, out);
	  if( brp->elsep != NULL ) {
		fprintf(out, " ");
		statement_as_lisp(brp->elsep, out);
	  }
	  fprintf(out, ") ");
	  break;
	}
    case FOR: {
	  for_s* fop = (for_s*)stat->child;
	  fprintf(out, "(basic-for %s ", fop->param);
	  expression_as_lisp(fop->start, out);
	  fprintf(out, " ");
	  expression_as_lisp(fop->stop, out);
	  fprintf(out, " ");
	  expression_as_lisp(fop->step, out);
	  fprintf(out, " ");
	  statement_as_lisp(fop->body, out);
	  fprintf(out, ") ");
	  break;
	}
    case WHILE: {
	  while_s* whp = (while_s*)stat->child;
	  fprintf(out, "(basic-while ");
	  expression_as_lisp(whp->cond, out);
	  fprintf(out, " ");
	  statement_as_lisp(whp->body, out);
	  fprintf(out, ") ");
	  break;
	}
    case CALL: {
	  call_s* cap = (call_s*)stat->child;
	  fprintf(out, "(basic-call %s ", cap->func->name);
	  node* ip = cap->argus;
	  while( ip != NULL ) {
		expression_as_lisp((expression*)(ip->data), out);
		ip = ip->next;
	  }
	  fprintf(out, ") ");
	  break;
	}
    case SEQ: {
	  sequence_s* sep = (sequence_s*)stat->child;
	  fprintf(out, "(basic-seq ");
	  node* sp = sep->elems;
	  while( sp != NULL ) {
		statement_as_lisp((statement*)(sp->data), out);
		sp = sp->next;
	  }
	  fprintf(out, ")");
	  break;
	}
  }
}

/**/
function* create_function( const char* nm, node* pr, statement* bo )
{
  function* subr = GC_MALLOC(sizeof(function));
  subr->name = GC_MALLOC(1 + strlen(nm));
  strcpy(subr->name, nm);
  subr->parameters = pr;
  subr->body = bo;
  return subr;
}

/**/
void function_as_lisp( function* subr, FILE* out )
{
  fprintf(out, "(basic-function %s ( ", subr->name);
  node* ip = subr->parameters;
  while( ip != NULL ) {
	expression_as_lisp((expression*)(ip->data), out);
	fprintf(out, " ");
	ip = ip->next;
  }
  fprintf(out, ") ");
  statement_as_lisp(subr->body, out);
  fprintf(out, ")");
}

/**/
void program_as_lisp( program* pro, FILE* out )
{
  node* ip = pro->subrs;
  while( ip != NULL ) {
	function_as_lisp((function*)(ip->data), out);
	fprintf(out, "\n");
	ip = ip->next;
  }
}

