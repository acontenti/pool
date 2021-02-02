lexer grammar PoolLexer;

options {
	language=Cpp;
}

WS : [ \t\r\n]+ -> channel(HIDDEN);
COMMENT : '/*' .*? '*/' -> channel(HIDDEN);
LINE_COMMENT : '//' ~[\r\n]* -> channel(HIDDEN);

SEMI:';';
LP:'(';
RP:')';
LB:'{';
RB:'}';
LSB:'[';
RSB:']';
DOT:'.';
COMMA:',';
COLON:':';

TRUE:'true';
FALSE:'false';
NULL_:'null';

fragment HEX_DIGIT: [0-9A-Fa-f];
fragment BIN_DIGIT: [0-1];
fragment DIGIT: [0-9];

DECIMAL_INTEGER_LITERAL: DIGIT+;
HEX_INTEGER_LITERAL: '0' [Xx] HEX_DIGIT*;
BIN_INTEGER_LITERAL: '0' [Bb] BIN_DIGIT*;
FLOAT_LITERAL: (DIGIT* '.')? DIGIT+;

fragment ESCAPE: '\\' ('"' | '\\' | '/' | 'b' | 'f' | 'n' | 'r' | 't' | ('u' HEX_DIGIT HEX_DIGIT HEX_DIGIT HEX_DIGIT));

STRING_LITERAL : '"' (~["\\\r\n] | ESCAPE)* '"';

IDENTIFIER: '$' ID;
OPERATOR: ID;
fragment ID: ([A-Za-z] | SYMBOL) ([A-Za-z0-9] | SYMBOL)*;
fragment SYMBOL: '_' | '!' | '%' | '&' | '*' | '+' | '-' | '/' | '<' | '>' | '=' | '^' | '|' | '~';