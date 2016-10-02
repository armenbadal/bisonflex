
%{
#include "ast.h"

#include "parser.tab.h"
%}

%option noyywrap
%option yylineno

number      [0-9]+(\.[0-9]+)?
ident       [a-zA-Z][a-zA-Z0-9]*
comment     \'.*$

%%
[ \t]       { /**/ }
{comment}   { /**/ }
{number}    {
              yylval.number = atof(yytext);
              return xNumber;
            }
"DECLARE"   { return xDeclare; }
"FUNCTION"  { return xFunction; }
"LET"       { return xLet; }
"INPUT"     { return xInput; }
"PRINT"     { return xPrint; }
"IF"        { return xIf; }
"THEN"      { return xThen; }
"ELSEIF"    { return xElseIf; }
"ELSE"      { return xElse; }
"END"       { return xEnd; }
"FOR"       { return xFor; }
"TO"        { return xTo; }
"STEP"      { return xStep; }
"WHILE"     { return xWhile; }
"CALL"      { return xCall; }
"OR"        { return xOr; }
"AND"       { return xAnd; }
"NOT"       { return xNot; }
{ident}     {
              yylval.name = clone_str(yytext);
              return xIdent;
            }
"="         { return xEq; }
"<>"        { return xNe; }
">"         { return xGt; }
">="        { return xGe; }
"<"         { return xLt; }
"<="        { return xLe; }
"+"         { return xAdd; }
"-"         { return xSub; }
"*"         { return xMul; }
"/"         { return xDiv; }
"^"         { return xPow; }
\n          { return xEol; }
.           { return (int)yytext[0]; }


