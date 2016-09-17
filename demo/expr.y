
%token xNumber
%token xIdent

%left xOr
%left xAnd
%nonassoc xEq xNe
%nonassoc xGt xGe xLt xLe
%left xAdd xSub
%left xMul xDiv
%right xPow
%right xNot

%start Expression
%%
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
	| '-' Expression %prec xNot
	| xNot Expression
	| xNumber
	| xIdent
	;

ArgumentList
    : ExpressionList
	| /* empty */
	;

ExpressionList
    : ExpressionList ',' Expression
	| Expression
	;


