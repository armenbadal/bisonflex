
#ifndef AST_H
#define AST_H

#include <stdio.h>

#include "slist.h"

/**/
typedef struct _function function;

/* արտահայտություններ */
typedef struct _expression expression;
struct _expression {
  // արտահայտության տեսակը
  enum {
    NUMBER,
    VARIABLE,
    UNARY,
    BINARY,
    APPLY,
  } kind;
  double number; // իրական թիվ
  char* name; // իդենտիֆիկատոր
  // գործողությունների կոդերը
  enum {
    OR, AND, EQ, NE, GT, GE,
    LT, LE, ADD, SUB, MUL,
    DIV, POW, NOT, NEG 
  } oper;
  expression* exo; // ենթաարտահայտություն
  expression* exi; // ենթաարտահայտություն
  node* args; // ֆունկցիայի կիրառման արգումենտներ
};

/* կոնստրուկտորներ */
extern expression* create_number( double );
extern expression* create_variable( const char* );
extern expression* create_unary( int, expression* );
extern expression* create_binary( int, expression*, expression* );
extern expression* create_apply( const char*, node* );

/* արտահայտության թարգմանությունը */
extern void expression_as_lisp( expression*, FILE* );

/* հրամաններ */
typedef struct _statement statement;
struct _statement {
  // հրամանի տեսակը
  enum {
    INPUT, PRINT, ASSIGN, IF,
    FOR, WHILE, CALL, SEQ,
  } kind;
  void* child; // հրամանի ցուցիչ
};

extern void statement_as_lisp( statement*, FILE* );

/* ներմուծում */
typedef struct _input_s input_s;
struct _input_s {
  char* vari;
};
extern statement* create_input( const char* );

/* արտածում */
typedef struct _print_s print_s;
struct _print_s {
  expression* valu;
};
extern statement* create_print( expression* );

/* վերագրում */
typedef struct _assign_s assign_s;
struct _assign_s {
  char* vari;
  expression* valu;
};
extern statement* create_assign( const char*, expression* );

/* ճյողավորում */
typedef struct _if_s if_s;
struct _if_s {
  expression* cond;
  statement* thenp;
  statement* elsep;
};
extern statement* create_if( expression*, statement*, statement*, statement* );

/* պարամետրով ցիկլ */
typedef struct _for_s for_s;
struct _for_s {
  char* param;
  expression* start;
  expression* stop;
  expression* step;
  statement* body;
};
extern statement* create_for( const char*, expression*, expression*, expression*, statement* );

/* նախապայմանով ցիկլ */
typedef struct _while_s while_s;
struct _while_s {
  expression* cond;
  statement* body;
};
extern statement* create_while( expression*, statement* );

/* պրոցեդուրայի կանչ */
typedef struct _call_s call_s;
struct _call_s {
  char* func;
  node* argus;
};
extern statement* create_call( const char*, node* );

/* հրամանների հաջորդում */
typedef struct _sequence_s sequence_s;
struct _sequence_s {
  node* elems;
};
extern statement* create_sequence( node* );

/* ֆունկցիա */
struct _function {
  char* name;
  node* parameters;
  statement* body;
};
extern function* create_function( const char*, node*, statement* );

extern void function_as_lisp( function*, FILE* );

/* ծրագիր */
typedef struct _program program;
struct _program {
  node* subrs;
};
extern function* function_by_name( program*, const char* );

extern void program_as_lisp( program*, FILE* );

/* օժանդակ գործիքներ */
extern char* clone_str( const char* );

#endif

