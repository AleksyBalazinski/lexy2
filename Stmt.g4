grammar Stmt;

import Expr;

statement: exprStatement | declStatement;

exprStatement: expr? ';';

declStatement: 'let' IDENTIFIER (':' TYPE_ID)? '=' expr;