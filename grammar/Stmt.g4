grammar Stmt;

import Expr;

statement: exprStatement | declStatement | printStatement;

exprStatement: expr ';';

printStatement: '__print' expr ';' # printIntrinsic;

declStatement: 'let' IDENTIFIER (':' TYPE_ID)? '=' expr ';';