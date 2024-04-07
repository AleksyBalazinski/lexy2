grammar Expr;

TYPE_ID: 'double' | 'int' | 'bool'; // TODO user defined types

INTEGER_LITERAL: [0-9]+;
FLOAT_LITERAL:
	[0-9]+ '.' [0-9]+ (('e' | 'E') ('+' | '-')? [0-9]+)?;
BOOL_LITERAL: 'true' | 'false';

IDENTIFIER: [a-zA-Z_][a-zA-Z0-9_]*;

WHITESPACE: [ \r\n\t]+ -> skip;
COMMENT: '//' ~[\r\n]* -> skip;

expr: assignmentExpr # fwd00 | expr ',' assignmentExpr # Comma;

assignmentExpr:
	conditionalExpr																# fwd01
	| IDENTIFIER op = ('=' | '+=' | '-=' | '*=' | '/=' | '%=') conditionalExpr	# Assign;

conditionalExpr:
	logicalOrExpr														# fwd05
	| cond = logicalOrExpr '?' then = expr ':' else = assignmentExpr	# Ternary;

logicalOrExpr:
	logicalAndExpr						# fwd07
	| logicalOrExpr '||' logicalAndExpr	# logicalOr;

logicalAndExpr:
	equalityExpr										# fwd10
	| left = logicalAndExpr '&&' right = equalityExpr	# LogicalAnd;

equalityExpr:
	relationalExpr													# fwd20
	| left = equalityExpr op = ('==' | '!=') right = relationalExpr	# Equality;

relationalExpr:
	additiveExpr																# fwd30
	| left = relationalExpr op = ('<' | '>' | '>=' | '<=') right = additiveExpr	# Relation;

additiveExpr:
	multiplicativeExpr													# fwd40
	| left = additiveExpr op = ('+' | '-') right = multiplicativeExpr	# Additive;

multiplicativeExpr:
	castExpr															# fwd50
	| left = multiplicativeExpr op = ('*' | '/' | '%') right = castExpr	# Multiplicative;

castExpr:
	unaryExpr							# fwd60
	| left = castExpr ' as ' TYPE_ID	# Cast;

unaryExpr:
	primaryExpr										# fwd70
	| op = ('+' | '-' | '!') right = primaryExpr	# Unary;

primaryExpr:
	literal			# fwd80
	| '(' expr ')'	# Parens
	| IDENTIFIER	# Idenitifer;

literal:
	INTEGER_LITERAL	# IntegerLiteral
	| FLOAT_LITERAL	# FloatLiteral
	| BOOL_LITERAL	# BoolLiteral;