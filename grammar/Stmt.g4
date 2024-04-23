grammar Stmt;

import Expr, Types;

statement:
	exprStatement
	| declStatement
	| printStatement
	| compoundStatement
	| selectionStatement
	| iterationStatement;

exprStatement: expr ';';

printStatement: '__print' expr ';' # printIntrinsic;

declStatement: variableDeclaration | functionDeclaration;

variableDeclaration: 'let' IDENTIFIER (':' type)? '=' expr ';';

functionDeclaration:
	'fn' IDENTIFIER '(' paramList ')' ('->' type)? functionBody;

paramList: param | paramList ',' param;

param: IDENTIFIER ':' type;

functionBody: compoundStatement;

returnStatement: 'return' expr;

compoundStatement: '{' statement+ '}';

condition: expr;

thenPart: compoundStatement;

elsePart:
	selectionStatement
	| compoundStatement; // allow for clean else-if

selectionStatement:
	'if' condition thenPart						# if
	| 'if' condition thenPart 'else' elsePart	# ifElse;

whileLoopBody: compoundStatement;

whileLoopCondition: expr;

iterationStatement:
	'while' whileLoopCondition whileLoopBody # whileLoop;