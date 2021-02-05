parser grammar PoolParser;

options {
	language=Cpp;
	tokenVocab=PoolLexer;
}

program: statement* EOF;

statement: (c=call | n=native)? SEMI;

native: BACKTICK IDENTIFIER (DOT IDENTIFIER)* BACKTICK;

call returns [enum Type {T,TA,TI,TIA,TO,TOC,TOA} type]:
	callee=call DOT IDENTIFIER args {$type=CallContext::TIA;} |
	callee=call DOT IDENTIFIER {$type=CallContext::TI;} |
    <assoc=right> callee=call args {$type=CallContext::TA;} |
	<assoc=right> callee=call OPERATOR args {$type=CallContext::TOA;} |
	<assoc=right> callee=call OPERATOR arg=call {$type=CallContext::TOC;} |
	<assoc=right> callee=call OPERATOR {$type=CallContext::TO;} |
	term {$type=CallContext::T;};

args: LP call? (COMMA call)* RP;

term returns [enum Type {NUM,STR,FUN,ARR,PAR,BLK,IDT} type]:
	num {$type=TermContext::NUM;} |
	string {$type=TermContext::STR;} |
	fun {$type=TermContext::FUN;} |
	array {$type=TermContext::ARR;} |
	par {$type=TermContext::PAR;} |
	block {$type=TermContext::BLK;} |
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
