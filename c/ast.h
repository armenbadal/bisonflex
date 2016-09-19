
#ifndef AST_H
#define AST_H

#include <stdio.h>

#include "slist.h"

/**/
struct function;

/**/
struct expression {
  enum {
	NUMBER,
	VARIABLE,
	UNARY,
	BINARY,
	APPLY,
  } kind;
  double number;
  char* ident;
  enum {
	OR, AND, EQ, NE, GT, GE,
	LT, LE, ADD, SUB, MUL,
	DIV, POW, NOT, NEG 
  } oper;
  struct expression* exo;
  struct expression* exi;
  struct function* func;
  struct node* exs;
};

/*
extern struct expression* create_number( double );
extern struct expression* create_variable( const char* );
extern struct expression* create_unary( int, struct expression* );
extern struct expression* create_binary( int, struct expression*, struct expression* );
extern struct expression* create_apply( struct function*, struct node* );
*/

extern void expression_as_lisp( struct expression*, FILE* );

/**/
struct statement;

/**/
struct input_s {
  char* vari;
};

/**/
struct print_s {
  struct expression* valu;
};

/**/
struct assignment_s {
  char* vari;
  struct expression* valu;
};

/**/
struct if_s {
  struct expression* cond;
  struct statement* thenp;
  struct statement* elsep;
};

/**/
struct for_s {
  char* param;
  struct expression* start;
  struct expression* stop;
  struct expression* step;
  struct statement* body;
};

/**/
struct while_s {
  struct expression* cond;
  struct statement* body;
};

/**/
struct call_s {
  struct function* func;
  struct node* argus;
};

/**/
struct sequence_s {
  struct node* elems;
};

/**/
struct statement {
  enum {
	INPUT, PRINT, ASSIGN, IF,
	FOR, WHILE, CALL, SEQ,
  } kind;
  void* child;
};

extern void statement_as_lisp( struct statement*, FILE* );

/**/
struct function {
  char* name;
  struct node* parameters;
  struct statement* body;
};

extern void function_as_lisp( struct function*, FILE* );

/**/
struct program {
  struct node* subs;
};

extern void program_as_lisp( struct program*, FILE* );

#endif

