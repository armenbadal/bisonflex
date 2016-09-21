
%{
#include <stdio.h>

extern int yylex();
static int yyerror( const char* );
%}

%error-verbose

%token xIdent
%token xNumber

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
	| /* empty */
	;

Function
    : xDeclare FunctionHeader
    | FunctionHeader StatementList xEnd xFunction NewLines
	;

FunctionHeader
    : xFunction xIdent '(' ParameterList ')' NewLines
	;

ParameterList
    : IdentifierList
	| /* empty */
	;

NewLines
    : NewLines xEol
	| xEol
	;

IdentifierList
    : IdentifierList ',' xIdent
	| xIdent
	;

StatementList
    : StatementList Statement NewLines
	| /* empty */
	;

Statement
    : xInput xIdent
	| xPrint Expression
	| LetOpt xIdent xEq Expression
	| xIf Expression xThen NewLines StatementList ElseIfPartList ElsePart xEnd xIf
	| xFor xIdent xEq Expression xTo Expression StepOpt NewLines StatementList xEnd xFor
	| xWhile Expression NewLines StatementList xEnd xWhile
	| xCall xIdent ArgumentList
	;

LetOpt
    : xLet
	| /* empty */
	;

ElseIfPartList
    : ElseIfPartList xElseIf Expression xThen NewLines StatementList
	| /* empty */
	;

ElsePart
    : xElse NewLines StatementList
	| /* empty */
	;

StepOpt
    : xStep Expression
	| /* empty */
	;

ArgumentList
    : ExpressionList
	| /* empty */
	;

ExpressionList
    : ExpressionList ',' Expression
	| Expression
	;

Expression
    : Expression xOr Expression
	| Expression xAnd Expression
	| Expression xEq Expression
	| Expression xNe Expression
	| Expression xGt Expression
	| Expression xGe Expression
	| Expression xLt Expression
	| Expression xLe Expression
	| Expression xAdd Expression
	| Expression xSub Expression
	| Expression xMul Expression
	| Expression xDiv Expression
	| Expression xPow Expression
	| '(' Expression ')'
	| xIdent '(' ArgumentList ')'
	| xSub Expression %prec xNot
	| xNot Expression
	| xNumber
	| xIdent
	;

%%

static int yyerror( const char* message )
{
  extern int yylineno;
  fprintf(stderr, "ՍԽԱԼ։ [%d] %s\n", yylineno, message);
  return 1;
}

