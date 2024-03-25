grammar Expr;

IDENTIFIER: [a-zA-Z0-9_]+;
TYPE_ID: [a-zA-Z0-9_]+;
INTEGER_LITERAL: [0-9]+;
FLOAT_LITERAL: [0-9]+ '.' [0-9]*;
BOOL_LITERAL: 'true' | 'false';

expr: assignmentExpr | expr ',' assignmentExpr;

assignmentExpr:
	IDENTIFIER assignmentOperator assignmentExpr
	| conditionalExpr;

assignmentOperator: '=' | '+=' | '*=' | '/=' | '%=';

conditionalExpr:
	logicalOrExpr
	| logicalOrExpr '?' expr ':' assignmentExpr;

logicalOrExpr:
	logicalAndExpr
	| logicalOrExpr '||' logicalAndExpr;

logicalAndExpr: equalityExpr | logicalAndExpr '&&' equalityExpr;

equalityExpr:
	relationalExpr
	| equalityExpr '==' relationalExpr
	| equalityExpr '!=' relationalExpr;

relationalExpr:
	additiveExpr
	| relationalExpr '<' additiveExpr
	| relationalExpr '>' additiveExpr
	| relationalExpr '>=' additiveExpr
	| relationalExpr '<=' additiveExpr;

additiveExpr:
	multiplicativeExpr
	| additiveExpr '+' multiplicativeExpr
	| additiveExpr '-' multiplicativeExpr;

multiplicativeExpr:
	castExpr
	| multiplicativeExpr '*' castExpr
	| multiplicativeExpr '/' castExpr
	| multiplicativeExpr '%' castExpr;

castExpr: unaryExpr | '(' TYPE_ID ')' castExpr;

unaryExpr:
	unaryOperator castExpr
	| IDENTIFIER '.' IDENTIFIER
	| IDENTIFIER '(' exprList ')'
	| primaryExpr;

unaryOperator: '+' | '-' | '!';

primaryExpr: literal | IDENTIFIER | '(' expr ')';

exprList: expr*;

literal: INTEGER_LITERAL | FLOAT_LITERAL | BOOL_LITERAL;