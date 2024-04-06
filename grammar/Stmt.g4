grammar Stmt;

import Expr;

statement:
	exprStatement
	| declStatement
	| printStatement
	| compoundStatement
	| selectionStatement;

exprStatement: expr ';';

printStatement: '__print' expr ';' # printIntrinsic;

declStatement: 'let' IDENTIFIER (':' TYPE_ID)? '=' expr ';';

compoundStatement: '{' statement+ '}';

condition: expr;

thenPart: compoundStatement;

elsePart: compoundStatement;

selectionStatement:
	'if' condition thenPart						# if
	| 'if' condition thenPart 'else' elsePart	# ifElse;