
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
  expr->name = clone_str(nm);
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
expression* create_apply( const char* fu, node* ps )
{
  expression* expr = GC_MALLOC(sizeof(expression));
  expr->kind = APPLY;
  expr->name = clone_str(fu);
  expr->args = ps;
  return expr;
}

/**/
static size_t indent = 1;
static void print_indent( FILE* out )
{
  for( size_t k = 0; k < indent; ++k )
	fprintf(out, "  ");
}
/**/
static void json_open_section( FILE* out, const char* name )
{
  print_indent(out);
  fprintf(out, "\"%s\" : {\n", name);
  ++indent;
}
static void json_close_section( FILE* out )
{
  fprintf(out, "\n");
  --indent;
  print_indent(out);
  fprintf(out, "}");
}
static void json_string_number( FILE* out, const char* name, double val )
{
  print_indent(out);
  fprintf(out, "\"%s\" : %lf", name, val);
}
static void json_string_string( FILE* out, const char* name, const char* val )
{
  print_indent(out);
  fprintf(out, "\"%s\" : \"%s\"", name, val);
}

/**/
void expression_as_json( expression* expr, FILE* out )
{
  if( expr == NULL )
    return;

  switch( expr->kind ) {
    case NUMBER:
      json_string_number(out, "number", expr->number);
      break;
    case VARIABLE:
      json_string_string(out, "variable", expr->name);
      break;
    case UNARY:
      if( NEG == expr->oper )
		json_open_section(out, "neg");
      else if( NOT == expr->oper )
        json_open_section(out, "not");
      expression_as_json(expr->exo, out);
	  json_close_section(out);
      break;
    case BINARY:
      switch( expr->oper ) {
        case OR:
          json_open_section(out, "or");
          break;
        case AND:
          json_open_section(out, "and");
          break;
        case EQ:
          json_open_section(out, "eq");
          break;
        case NE:
          json_open_section(out, "ne");
          break;
        case GT:
          json_open_section(out, "gt");
          break;
        case GE:
          json_open_section(out, "ge");
          break;
        case LT:
          json_open_section(out, "lt");
          break;
        case LE:
          json_open_section(out, "le");
          break;
        case ADD:
          json_open_section(out, "add");
          break;
        case SUB:
          json_open_section(out, "sub");
          break;
        case MUL:
          json_open_section(out, "mul");
          break;
        case DIV:
          json_open_section(out, "div");
          break;
        case POW:
          json_open_section(out, "pow");
          break;
      }
      expression_as_json(expr->exo, out);
      fprintf(out, ",\n");
      expression_as_json(expr->exi, out);
	  json_close_section(out);
      break;
    case APPLY: {
      json_open_section(out, "apply");
	  json_string_string(out, "proc", expr->name);
      fprintf(out, ",\n");
      node* ip = expr->args; // REVIEW
      while( ip != NULL ) {
        expression_as_json((expression*)(ip->data), out);
        ip = ip->next;
      }
      json_close_section(out);
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
  inp->vari = clone_str(vr);
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
  asi->vari = clone_str(vr);
  asi->valu = vl;
  return create_statement(ASSIGN, asi);
}

/**/
statement* create_if( expression* co, statement* tp, statement* eis, statement* ep )
{
  statement* sp = eis;
  if( sp != NULL ) {
    if_s* p = (if_s*)(sp->child);
    while( p->elsep != NULL )
      p = (if_s*)(p->elsep->child);
    p->elsep = ep;
  }
  else
    sp = ep;
  
  if_s* ifp = GC_MALLOC(sizeof(if_s));
  ifp->cond = co;
  ifp->thenp = tp;
  ifp->elsep = sp;
  return create_statement(IF, ifp);
}

/**/
statement* create_for( const char* pr, expression* sa, expression* se, expression* sp, statement* bo )
{
  for_s* fop = GC_MALLOC(sizeof(for_s));
  fop->param = clone_str(pr);
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
statement* create_call( const char* fu, node* ag )
{
  call_s* cap = GC_MALLOC(sizeof(call_s));
  cap->func = clone_str(fu);
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
void statement_as_json( statement* stat, FILE* out )
{
  if( stat == NULL )
    return;
  
  switch( stat->kind ) {
    case INPUT: {
      input_s* inp = (input_s*)stat->child;
      json_open_section(out, "input");
	  json_string_string(out, "variable", inp->vari);
	  json_close_section(out);
      break;
    }
    case PRINT: {
      print_s* prp = (print_s*)stat->child;
      json_open_section(out, "print");
      expression_as_json(prp->valu, out);
	  json_close_section(out);
      break;
    }
    case ASSIGN: {
      assign_s* asp = (assign_s*)stat->child;
      json_open_section(out, "assign");
	  json_string_string(out, "variable", asp->vari);
      fprintf(out, ",\n");
      expression_as_json(asp->valu, out);
	  json_close_section(out);
      break;
    }
    case IF: {
      if_s* brp = (if_s*)stat->child;
      json_open_section(out, "if");
      expression_as_json(brp->cond, out);
      fprintf(out, ",\n");
      statement_as_json(brp->thenp, out);
      if( brp->elsep != NULL ) {
        fprintf(out, ",\n");
        statement_as_json(brp->elsep, out);
      }
      json_close_section(out);
      break;
    }
    case FOR: {
      for_s* fop = (for_s*)stat->child;
      json_open_section(out, "for");
	  json_string_string(out, "variable", fop->param);
	  fprintf(out, ",\n");
      expression_as_json(fop->start, out);
      fprintf(out, ",\n");
      expression_as_json(fop->stop, out);
      fprintf(out, ",\n");
      if( fop->step == NULL )
		json_string_number(out, "number", 1);
	  else
		expression_as_json(fop->step, out);
      fprintf(out, ",\n");
      statement_as_json(fop->body, out);
      json_close_section(out);
      break;
    }
    case WHILE: {
      while_s* whp = (while_s*)stat->child;
      json_open_section(out, "while");
      expression_as_json(whp->cond, out);
      fprintf(out, ",\n");
      statement_as_json(whp->body, out);
      json_close_section(out);
      break;
    }
    case CALL: {
      call_s* cap = (call_s*)stat->child;
      json_open_section(out, "call");
	  json_string_string(out, "proc", cap->func);
      fprintf(out, ",\n");
      node* ip = cap->argus;
      while( ip != NULL ) {
        expression_as_json((expression*)(ip->data), out);
		fprintf(out, ",\n");
        ip = ip->next;
      }
      json_close_section(out);
      break;
    }
    case SEQ: {
      sequence_s* sep = (sequence_s*)stat->child;
      if( sep->elems->next == NULL )
		statement_as_json((statement*)(sep->elems->data), out);
      else {
		json_open_section(out, "seq");
		node* sp = sep->elems;
		while( sp != NULL ) {
		  statement_as_json((statement*)(sp->data), out);
		  sp = sp->next;
		  if( sp != NULL )
			fprintf(out, ",\n");
		}
		json_close_section(out);
      }
      break;
    }
  }
}

/**/
function* create_function( const char* nm, node* pr, statement* bo )
{
  function* subr = GC_MALLOC(sizeof(function));
  subr->name = clone_str(nm);
  subr->parameters = pr;
  subr->body = bo;
  return subr;
}

/**/
void function_as_json( function* subr, FILE* out )
{
  json_open_section(out, "function");
  json_string_string(out, "name", subr->name);
  fprintf(out, ",\n");
  json_open_section(out, "parameters");
  node* ip = subr->parameters;
  while( ip != NULL ) {
    json_string_string(out, "variable", (char*)(ip->data));
    ip = ip->next;
	if( ip != NULL )
	  fprintf(out, ",\n");
  }
  json_close_section(out);
  fprintf(out, ",\n");
  statement_as_json(subr->body, out);
  json_close_section(out);
}

/**/
function* function_by_name( program* pro, const char* nm )
{
  node* np = pro->subrs;
  while( np != NULL ) {
	function* fp = (function*)(np->data);
	if( 0 == strcmp(fp->name, nm) )
	  break;
	np = np->next;
  }
  return np == NULL ? NULL : np->data;
}

/**/
void program_as_json( program* pro, FILE* out )
{
  fprintf(out, "{\n");
  node* ip = pro->subrs;
  while( ip != NULL ) {
    function_as_json((function*)(ip->data), out);
    ip = ip->next;
    if( ip != NULL )
	  fprintf(out, "\n");
  }
  fprintf(out, "\n}\n");
}

/**/
char* clone_str( const char* so )
{
  char* res = GC_MALLOC(1 + strlen(so));
  strcpy(res, so);
  return res;
}


