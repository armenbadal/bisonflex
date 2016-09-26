/*
*/

%{
#include <stdio.h>

#include "ast.h"
#include "slist.h"
  
extern int yylex();
static int yyerror( const char* );
 
extern int yylineno;
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

%type <list> FunctionList
%type <func> Function
%type <func> FunctionHeader
%type <list> ParameterList
%type <list> IdentifierList
%type <stat> StatementList
%type <stat> Statement
%type <list> ElseIfPartList
%type <stat> ElsePart
%type <expr> StepOpt
%type <list> ArgumentList
%type <list> ExpressionList
%type <expr> Expression

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
    | /* empty */
    ;

FunctionList
    : FunctionList Function
	{
	  // TODO
	}
    | /* empty */
	{
	  $$ = NULL;
	}
    ;

Function
    : xDeclare FunctionHeader
	{
	  $$ = $2;
	}
    | FunctionHeader StatementList xEnd xFunction NewLines
	{
	  $$ = $1;
	  $$->body = $2;
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
    | /* empty */
	{
	  $$ = NULL;
	}
    ;

NewLines
    : NewLines xEol
    | xEol
    ;

IdentifierList
    : IdentifierList ',' xIdent
	{
	  // TODO
	}
    | xIdent
	{
	  // TODO
	}
    ;

StatementList
    : StatementList Statement NewLines
	{
	  // TODO
	}
    | /* empty */
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
	  statement* elp = NULL; // լրացնել
	  $$ = create_if($2, $5, elp);
	}
    | xFor xIdent xEq Expression xTo Expression StepOpt NewLines StatementList xEnd xFor
	{
	  $$ = create_for($2, $4, $6, $7, $9);
	}
    | xWhile Expression NewLines StatementList xEnd xWhile
	{
	  $$ = create_while($2, $4);
	}
    | xCall xIdent ArgumentList
	{
	  function* calee = NULL; // որոնել ֆունկցիան ծրագրում
	  $$ = create_call(calee, $3);
	}
    ;

LetOpt
    : xLet
	{
	  // TODO
	}
    | /* empty */
	{
	  // TODO
	}
    ;

ElseIfPartList
    : ElseIfPartList xElseIf Expression xThen NewLines StatementList
	{}
    | /* empty */
	{}
    ;

ElsePart
    : xElse NewLines StatementList
	{
	  $$ = $3;
	}
    | /* empty */
	{
	  $$ = NULL;
	}
    ;

StepOpt
    : xStep Expression
	{}
    | /* empty */
	{}
    ;

ArgumentList
    : ExpressionList
	{
	  $$ = $1;
	}
    | /* empty */
	{
	  $$ = NULL;
	}
    ;

ExpressionList
    : ExpressionList ',' Expression
	{
	  $$ = append_to($1, $3);
	}
    | Expression
	{
	  $$ = create_node($1);
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
	  function* calee = NULL; // որոնել ֆունկցիան ծրագրի մեջ
	  $$ = create_apply(calee, $3);
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

