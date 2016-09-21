
#ifndef AST_H
#define AST_H

#include <stdio.h>

#include "slist.h"

/**/
//struct function;
typedef struct _function function;

/**/
typedef struct _expression expression;
struct _expression {
  enum {
	NUMBER,
	VARIABLE,
	UNARY,
	BINARY,
	APPLY,
  } kind;
  double number;
  char* name;
  enum {
	OR, AND, EQ, NE, GT, GE,
	LT, LE, ADD, SUB, MUL,
	DIV, POW, NOT, NEG 
  } oper;
  expression* exo;
  expression* exi;
  function* func;
  node* exs;
};

extern expression* create_number( double );
extern expression* create_variable( const char* );
extern expression* create_unary( int, expression* );
extern expression* create_binary( int, expression*, expression* );
extern expression* create_apply( function*, node* );

extern void expression_as_lisp( expression*, FILE* );

/**/
//struct _statement;
typedef struct _statement statement;

/**/
typedef struct _input_s input_s;
struct _input_s {
  char* vari;
};
extern statement* create_input( const char* );

/**/
typedef struct _print_s print_s;
struct _print_s {
  expression* valu;
};
extern statement* create_print( expression* );

/**/
typedef struct _assign_s assign_s;
struct _assign_s {
  char* vari;
  expression* valu;
};
extern statement* create_assign( const char*, expression* );

/**/
typedef struct _if_s if_s;
struct _if_s {
  expression* cond;
  statement* thenp;
  statement* elsep;
};
extern statement* create_if( expression*, statement*, statement* );

/**/
typedef struct _for_s for_s;
struct _for_s {
  char* param;
  expression* start;
  expression* stop;
  expression* step;
  statement* body;
};
extern statement* create_for( const char*, expression*, expression*, expression*, statement* );


/**/
typedef struct _while_s while_s;
struct _while_s {
  expression* cond;
  statement* body;
};
extern statement* create_while( expression*, statement* );

/**/
typedef struct _call_s call_s;
struct _call_s {
  function* func;
  node* argus;
};
extern statement* create_call( function*, node* );

/**/
typedef struct _sequence_s sequence_s;
struct _sequence_s {
  node* elems;
};
extern statement* create_sequence( node* );

/**/
typedef struct _statement statement;
struct _statement {
  enum {
	INPUT, PRINT, ASSIGN, IF,
	FOR, WHILE, CALL, SEQ,
  } kind;
  void* child;
};

extern void statement_as_lisp( statement*, FILE* );

/**/
struct _function {
  char* name;
  node* parameters;
  statement* body;
};
extern function* create_function( const char*, node*, statement* );

extern void function_as_lisp( function*, FILE* );

/**/
typedef struct _program program;
struct _program {
  node* subrs;
};

extern void program_as_lisp( program*, FILE* );

#endif

