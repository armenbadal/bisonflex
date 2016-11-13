/*
*/

%{
#include <stdio.h>

#include <gc.h>

#include "ast.h"
#include "slist.h"
  
extern int yylex();
static int yyerror( const char* );
 
extern int yylineno;

extern program* prog;
%}

%error-verbose

%union {
  double number;    // իրական թիվ
  char* name;       // իդենտիֆիկատոր
  expression* expr; // արտահայտություն
  statement* stat;  // հրաման
  function* func;   // ֆունկցիա
  node* list;       // ցուցակ
}

%token <name> xIdent
%token <number> xNumber

%token xDeclare
%token xFunction
%token xLet
%token xInput
%token xPrint
%token xIf
%token xThen
%token xElseIf
%token xElse
%token xEnd
%token xFor
%token xTo
%token xStep
%token xWhile
%token xCall

%left xOr
%left xAnd
%nonassoc xEq xNe
%nonassoc xGt xGe xLt xLe
%left xAdd xSub
%left xMul xDiv
%right xPow
%right xNot

%token xEol

%type <func> Function
%type <func> FunctionHeader

%type <stat> Statement
%type <stat> ElsePart

%type <expr> Expression
%type <expr> StepOpt

%type <list> ParameterList
%type <list> IdentifierList
%type <list> StatementList
%type <stat> ElseIfPartList
%type <list> ArgumentList
%type <list> ExpressionList


%start Program
%%
Program
    : NewLinesOpt FunctionList
    {
      puts("Parsed");
    }
    ;

NewLinesOpt
    : NewLines
    | %empty
    ;

FunctionList
    : FunctionList Function
    | %empty
    ;

Function
    : xDeclare FunctionHeader
    {
      $$ = $2;
      prog->subrs = append_to(prog->subrs, $$);
    }
    | FunctionHeader StatementList xEnd xFunction NewLines
    {
      function* fp = function_by_name(prog, $1->name);
      if( fp == NULL )
        prog->subrs = append_to(prog->subrs, $1);
      $1->body = create_sequence($2);
      $$ = $1;
    }
    ;

FunctionHeader
    : xFunction xIdent '(' ParameterList ')' NewLines
    {
      $$ = create_function($2, $4, NULL);
    }
    ;

ParameterList
    : IdentifierList
    {
      $$ = $1;
    }
    | %empty
    {
      $$ = NULL;
    }
    ;

NewLines
    : NewLines xEol
    | xEol
    ;

IdentifierList
    : xIdent ',' IdentifierList
    {
      $$ = create_node($1, $3);
    }
    | xIdent
    {
      $$ = create_node($1, NULL);
    }
    ;

StatementList
    : Statement NewLines StatementList
    {
      $$ = create_node($1, $3);
    }
    | %empty
    {
      $$ = NULL;
    }
    ;

Statement
    : xInput xIdent
    {
      $$ = create_input($2);
    }
    | xPrint Expression
    {
      $$ = create_print($2);
    }
    | LetOpt xIdent xEq Expression
    {
      $$ = create_assign($2, $4);
    }
    | xIf Expression xThen NewLines StatementList ElseIfPartList ElsePart xEnd xIf
    {
      $$ = create_if($2, create_sequence($5), $6, $7);
    }
    | xFor xIdent xEq Expression xTo Expression StepOpt NewLines StatementList xEnd xFor
    {
      $$ = create_for($2, $4, $6, $7, create_sequence($9));
    }
    | xWhile Expression NewLines StatementList xEnd xWhile
    {
      $$ = create_while($2, create_sequence($4));
    }
    | xCall xIdent ArgumentList
    {
      $$ = create_call($2, $3);
    }
    ;

LetOpt
    : xLet
    | %empty
    ;

ElseIfPartList
    : xElseIf Expression xThen NewLines StatementList ElseIfPartList 
    {
      $$ = create_if($2, create_sequence($5), $6, NULL);
    }
    | %empty
    {
      $$ = NULL;
    }
    ;

ElsePart
    : xElse NewLines StatementList
    {
      $$ = create_sequence($3);
    }
    | %empty
    {
      $$ = NULL;
    }
    ;

StepOpt
    : xStep Expression
    {
      $$ = $2;
    }
    | %empty
    {
      $$ = NULL;
    }
    ;

ArgumentList
    : ExpressionList
    {
      $$ = $1;
    }
    | %empty
    {
      $$ = NULL;
    }
    ;

ExpressionList
    : Expression ',' ExpressionList
    {
      $$ = create_node($1, $3);
    }
    | Expression
    {
      $$ = create_node($1, NULL);
    }
    ;

Expression
    : Expression xOr Expression
    {
      $$ = create_binary(OR, $1, $3);
    }
    | Expression xAnd Expression
    {
      $$ = create_binary(AND, $1, $3);
    }
    | Expression xEq Expression
    {
      $$ = create_binary(EQ, $1, $3);
    }
    | Expression xNe Expression
    {
      $$ = create_binary(NE, $1, $3);
    }
    | Expression xGt Expression
    {
      $$ = create_binary(GT, $1, $3);
    }
    | Expression xGe Expression
    {
      $$ = create_binary(GE, $1, $3);
    }
    | Expression xLt Expression
    {
      $$ = create_binary(LT, $1, $3);
    }
    | Expression xLe Expression
    {
      $$ = create_binary(LE, $1, $3);
    }
    | Expression xAdd Expression
    {
      $$ = create_binary(ADD, $1, $3);
    }
    | Expression xSub Expression
    {
      $$ = create_binary(SUB, $1, $3);
    }
    | Expression xMul Expression
    {
      $$ = create_binary(MUL, $1, $3);
    }
    | Expression xDiv Expression
    {
      $$ = create_binary(DIV, $1, $3);
    }
    | Expression xPow Expression
    {
      $$ = create_binary(POW, $1, $3);
    }
    | '(' Expression ')'
    {
      $$ = $2;
    }
    | xIdent '(' ArgumentList ')'
    {
      $$ = create_apply($1, $3);
    }
    | xSub Expression %prec xNot
    {
      $$ = create_unary(NEG, $2);
    }
    | xNot Expression
    {
      $$ = create_unary(NOT, $2);
    }
    | xNumber
    {
      $$ = create_number($1);
    }
    | xIdent
    {
      $$ = create_variable($1);
    }
    ;

%%

static int yyerror( const char* message )
{
  fprintf(stderr, "ERROR: (%d) %s\n", yylineno, message);
  return 1;
}

