parser grammar PoolParser;

options {
	language=Cpp;
	tokenVocab=PoolLexer;
}

program: statement* EOF;

statement: (call)? SEMI;

call returns [enum Type {AA,A,LAA,LA,IA,PA,O,OA,OC,T} type]:
	callee=call DOT IDENTIFIER args {$type=CallContext::AA;} |
	callee=call DOT IDENTIFIER {$type=CallContext::A;} |
	callee=call SQ IDENTIFIER args {$type=CallContext::LAA;} |
	callee=call SQ IDENTIFIER {$type=CallContext::LA;} |
    <assoc=right> IDENTIFIER args {$type=CallContext::IA;} |
    <assoc=right> LP callee=call RP args {$type=CallContext::PA;} |
	<assoc=right> callee=call OPERATOR {$type=CallContext::O;} |
	<assoc=right> callee=call OPERATOR args {$type=CallContext::OA;} |
	<assoc=right> callee=call OPERATOR arg=call {$type=CallContext::OC;} |
	term {$type=CallContext::T;};

args: LP call? (COMMA call)* RP;

term returns [enum Type {NUM,STR,FUN,ARR,PAR,BLK,NSM,IDT} type]:
	num {$type=TermContext::NUM;} |
	string {$type=TermContext::STR;} |
	fun {$type=TermContext::FUN;} |
	array {$type=TermContext::ARR;} |
	par {$type=TermContext::PAR;} |
	block {$type=TermContext::BLK;} |
	NATIVE_SYMBOL {$type=TermContext::NSM;} |
	IDENTIFIER {$type=TermContext::IDT;};

par: LP call? RP;

block: LB statement* RB;

array: LSB call? (COMMA + call)* RSB;

fun: (IDENTIFIER | (LP IDENTIFIER? (COMMA IDENTIFIER)* RP)) COLON LB statement* RB;

num returns [enum Type {DEC,HEX,BIN,FLT} type]:
	DECIMAL_INTEGER_LITERAL {$type=NumContext::DEC;} |
	HEX_INTEGER_LITERAL {$type=NumContext::HEX;} |
	BIN_INTEGER_LITERAL {$type=NumContext::BIN;} |
	FLOAT_LITERAL  {$type=NumContext::FLT;};

string: STRING_LITERAL;
