grammar Types;

import Expr;

type: arrayType | pointerType | simpleType;

arrayType: nonArrayType rankSpecifier+;

rankSpecifier: '[' INTEGER_LITERAL ']';

simpleType: IDENTIFIER;

nonArrayType: pointerType | simpleType;

pointerType: simpleType '*'+ | 'void' '*'+;