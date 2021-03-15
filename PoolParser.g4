parser grammar PoolParser;

options {
	language=Cpp;
	tokenVocab=PoolLexer;
}

program: statement* EOF;

statement: expression? SEMI;

expression: a=assignment | c=call;

assignment returns [enum Type {V,C} type]:
	<assoc=right> assignee=access EQ value=call {$type=AssignmentContext::V;} |
	<assoc=right> assignee=access CEQ value=call {$type=AssignmentContext::C;};

access returns [enum Type {G,L,I} type]:
	callee=call DOT ID {$type=AccessContext::G;} |
	callee=call SQ ID {$type=AccessContext::L;} |
	ID {$type=AccessContext::I;};

call returns [enum Type {IA,A,T} type]:
	callee=call DOT ID a=args? {$type=CallContext::IA;} |
    callee=call a=args {$type=CallContext::A;} |
	term {$type=CallContext::T;};

args: LP call? (COMMA call)* RP;

term returns [enum Type {NUM,STR,FUN,PAR,BLK,NSM,IDT} type]:
	num {$type=TermContext::NUM;} |
	string {$type=TermContext::STR;} |
	fun {$type=TermContext::FUN;} |
	par {$type=TermContext::PAR;} |
	block {$type=TermContext::BLK;} |
	NATIVE_SYMBOL {$type=TermContext::NSM;} |
	ID {$type=TermContext::IDT;};

par: LP expression RP;

block: LB statement* RB;

fun: LP ID? (COMMA ID)* RP COLON LB statement* RB;

num returns [enum Type {DEC,HEX,BIN,FLT} type]:
	DECIMAL_INTEGER_LITERAL {$type=NumContext::DEC;} |
	HEX_INTEGER_LITERAL {$type=NumContext::HEX;} |
	BIN_INTEGER_LITERAL {$type=NumContext::BIN;} |
	FLOAT_LITERAL  {$type=NumContext::FLT;};

string: STRING_LITERAL;
