parser grammar PoolParser;

options {
	language=Cpp;
	tokenVocab=PoolLexer;
}

pool: statement*;

statement: call? SEMI;

call: (caller=invocation? op=OPERATOR callee=invocation) | (caller=invocation op=OPERATOR? callee=invocation?);

invocation: access args?;

access: term (DOT ids+=IDENTIFIER)*;

term: null | num | boolean | string | fun | tuple | array | par | block | id;

par: LP call? RP;

block: LB statement* RB;

tuple: LP (call COMMA)+ call? RP;

array: LSB call? (COMMA + call)* RSB;

fun: params COLON block;

params: ids+=IDENTIFIER | (LP ids+=IDENTIFIER? (COMMA ids+=IDENTIFIER)* RP);

args: LP calls+=call? (COMMA calls+=call)* RP;

id: IDENTIFIER;

num: DECIMAL_INTEGER_LITERAL | HEX_INTEGER_LITERAL | BIN_INTEGER_LITERAL | FLOAT_LITERAL;

string: STRING_LITERAL;

boolean: TRUE | FALSE;

null: NULL_;
