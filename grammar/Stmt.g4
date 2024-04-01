grammar Stmt;

import Expr;

statement:
	exprStatement
	| declStatement
	| printStatement
	| compoundStatement;

exprStatement: expr ';';

printStatement: '__print' expr ';' # printIntrinsic;

declStatement: 'let' IDENTIFIER (':' TYPE_ID)? '=' expr ';';

compoundStatement: '{' statementSeq '}';

statementSeq: statement | statementSeq statement;