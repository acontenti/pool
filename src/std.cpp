#include "std.hpp"

const std::string std_module = R"::::(
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
Bool..toString ::= `Bool.toString`;
Bool..! ::= `Bool.!`;
Bool..|| ::= `Bool.||`;
Bool..&& ::= `Bool.&&`;
Bool..== ::= `Bool.==`;
true ::= `true`;
false ::= `false`;

Array ::= `Array`;
Array..init ::= `Array.init`;
Array..at ::= `Array.at`;
Array..forEach ::= `Array.forEach`;
Array..push ::= `Array.push`;
Array..toString ::= `Array.toString`;

Integer ::= `Integer`;
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
	i := 0.+(this);
	array := Array.new();
	end;
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
String..+ ::= `String.+`;
String..== ::= `String.==`;
String..toString ::= `String.toString`;

Block ::= `Block`;
Block..whileDo ::= `Block.whileDo`;

Fun ::= `Fun`;
Fun..classmethod ::= `Fun.classmethod`;
Fun..staticmethod ::= `Fun.staticmethod`;
Fun..toString ::= `Fun.toString`;

Void ::= `Void`;
Void..toString ::= (this) : {"void";};
void ::= `void`;

Nothing ::= `Nothing`;
Nothing..toString ::= (this) : {"null";};
null ::= `null`;

assert ::= `assert`;
)::::";

const std::unordered_map<std::string, std::string> STD_MODULES = {
	{":std", std_module}
};

