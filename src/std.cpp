#include "std.hpp"

const std::string std_module = R"::::(
null ::= `null`;
void ::= `void`;
true ::= `true`;
false ::= `false`;

Class ::= `Class`;
Class..extend ::= `Class.extend`;
Class..new ::= `Class.new`;
Class..toString ::= `Class.toString`;

Object ::= `Object`;
Object..toString ::= `Object.toString`;
Object..type ::= `Object.type`;
Object..println ::= `Object.println`;
Object..!= ::= (this, other) : {
	this.==(other).!();
};

Bool ::= `Bool`;
Bool..new ::= null;
Bool..toString ::= `Bool.toString`;
Bool..! ::= `Bool.!`;
Bool..|| ::= `Bool.||`;
Bool..&& ::= `Bool.&&`;
Bool..== ::= `Bool.==`;

Array ::= `Array`;
Array..init ::= `Array.init`;
Array..at ::= `Array.at`;
Array..forEach ::= `Array.forEach`;
Array..push ::= `Array.push`;
Array..toString ::= `Array.toString`;

Integer ::= `Integer`;
Integer..new ::= null;
Integer..+ ::= `Integer.+`;
Integer..- ::= `Integer.-`;
Integer..== ::= `Integer.==`;
Integer..< ::= `Integer.<`;
Integer..<= ::= (this, other) : {
	this.<(other).||(this.==(other)).!();
};
Integer..> ::= (this, other) : {
	this.<=(other).!();
};
Integer..>= ::= (this, other) : {
	this.<(other).!();
};
Integer..toString ::= `Integer.toString`;
Integer..to ::= (this, end) : {
	i := this;
	array := Array.new();
	{i.<(end);}.whileDo({
		array.push(i);
		i := i.+(1);
	});
	array;
};
Integer..toInclusive ::= (this, end) : {
	this.to(end.+(1));
};

Decimal ::= `Decimal`;
Decimal..new ::= null;
Decimal..+ ::= `Decimal.+`;
Decimal..- ::= `Decimal.-`;
Decimal..== ::= `Decimal.==`;
Decimal..< ::= `Decimal.<`;
Decimal..<= ::= (this, other) : {
	this.<(other).||(this.==(other)).!();
};
Decimal..> ::= (this, other) : {
	this.<=(other).!();
};
Decimal..>= ::= (this, other) : {
	this.<(other).!();
};
Decimal..toString ::= `Decimal.toString`;

String ::= `String`;
String..new ::= null;
String..+ ::= `String.+`;
String..== ::= `String.==`;
String..toString ::= `String.toString`;

Block ::= `Block`;
Block..new ::= null;
Block..whileDo ::= `Block.whileDo`;

Fun ::= `Fun`;
Fun..new ::= null;
Fun..classmethod ::= `Fun.classmethod`;
Fun..staticmethod ::= `Fun.staticmethod`;
Fun..toString ::= `Fun.toString`;

Void ::= `Void`;
Void..new ::= null;
Void..toString ::= (this) : {"void";};

Nothing ::= `Nothing`;
Nothing..new ::= null;
Nothing..toString ::= (this) : {"null";};

assert ::= `assert`;
)::::";

const std::unordered_map<std::string, std::string> STD_MODULES = {
	{":std", std_module}
};

