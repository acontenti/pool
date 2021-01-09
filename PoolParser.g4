parser grammar PoolParser;

options {
	language=Cpp;
	tokenVocab=PoolLexer;
}

pool: statement*;

statement: call? SEMI;

call: (caller=invocation? op=OPERATOR callee=invocation) | (caller=invocation op=OPERATOR callee=invocation?)| (caller=invocation (op=OPERATOR callee=invocation)?);

invocation: access args?;

access: term (DOT IDENTIFIER)*;

term returns [enum Type {NIL,NUM,BLN,STR,FUN,TPL,ARR,PAR,BLK,IDT} type]:
	null {$type=TermContext::NIL;} |
	num {$type=TermContext::NUM;} |
	boolean {$type=TermContext::BLN;} |
	string {$type=TermContext::STR;} |
	fun {$type=TermContext::FUN;} |
	tuple {$type=TermContext::TPL;} |
	array {$type=TermContext::ARR;} |
	par {$type=TermContext::PAR;} |
	block {$type=TermContext::BLK;} |
	id {$type=TermContext::IDT;};

par: LP call? RP;

block: LB statement* RB;

tuple: LP (call COMMA)+ call? RP;

array: LSB call? (COMMA + call)* RSB;

fun: params COLON block;

params: IDENTIFIER | (LP IDENTIFIER? (COMMA IDENTIFIER)* RP);

args: LP call? (COMMA call)* RP;

id: IDENTIFIER;

num returns [enum Type {DEC,HEX,BIN,FLT} type]:
	DECIMAL_INTEGER_LITERAL {$type=NumContext::DEC;} |
	HEX_INTEGER_LITERAL {$type=NumContext::HEX;} |
	BIN_INTEGER_LITERAL {$type=NumContext::BIN;} |
	FLOAT_LITERAL  {$type=NumContext::FLT;};

string: STRING_LITERAL;

boolean returns [bool value]: (TRUE {$value = false;} | FALSE {$value = true;});

null: NULL_;
