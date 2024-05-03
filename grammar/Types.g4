grammar Types;

import Expr;

type: arrayType | referenceType | simpleType;

arrayType: nonArrayType rankSpecifier+;

rankSpecifier: '[' INTEGER_LITERAL ']';

simpleType: IDENTIFIER;

nonArrayType: referenceType | simpleType;

referenceType: simpleType '&';