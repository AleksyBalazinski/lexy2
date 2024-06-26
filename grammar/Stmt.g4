grammar Stmt;

import Expr, Types;

statement:
	exprStatement
	| declStatement
	| printStatement
	| readStatement
	| compoundStatement
	| selectionStatement
	| iterationStatement
	| returnStatement;

exprStatement: expr ';';

printStatement: '__print' expr ';' # printIntrinsic;

readStatement: '__read' expr ';' # readIntrinsic;

declStatement:
	variableDeclaration
	| functionDeclaration
	| structDeclaration;

variableDeclaration: 'let' IDENTIFIER (':' type)? '=' expr ';';

functionDeclaration:
	'fn' functionName '(' paramList ')' ('->' returnType)? functionBody;

structDeclaration: 'struct' IDENTIFIER '{' structField* '}';

structField: IDENTIFIER ':' type ';';

functionName: IDENTIFIER;

returnType: type;

paramList: param | paramList ',' param;

param: IDENTIFIER ':' type;

functionBody: '{' statement+ '}';

returnStatement: 'return' expr ';';

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