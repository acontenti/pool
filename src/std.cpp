#include "std.hpp"

const std::string std_module = R"::::(
Class ::= `Class`;
Class::extend ::= `Class.extend`;
Class::new ::= `Class.new`;
Class::toString ::= `Class.toString`;

Object ::= `Object`;
Object::toString ::= `Object.toString`;
Object::type ::= `Object.type`;
Object::println ::= `Object.println`;

Bool ::= `Bool`;
Bool::toString ::= `Bool.toString`;
Bool::! ::= `Bool.!`;
true ::= `true`;
false ::= `false`;

Integer ::= `Integer`;
Integer::+ ::= `Integer.+`;
Integer::- ::= `Integer.-`;
Integer::< ::= `Integer.<`;
Integer::> ::= (this, other) : {
	this.<(other).!();
};
Integer::toString ::= `Integer.toString`;
Integer::to ::= (this, end) : {
	i := this.+(0);
	array := Array.new();
	end.println();
	{i.<(end);}.whileDo({
		array.<<(i);
		i := i.+(1);
	});
	array;
};

String ::= `String`;
String::+ ::= `String.+`;
String::toString ::= (this) : {this;};

Decimal ::= `Decimal`;
Decimal::+ ::= `Decimal.+`;
Decimal::- ::= `Decimal.-`;
Decimal::toString ::= `Decimal.toString`;

Array ::= `Array`;
Array::init ::= `Array.init`;
Array::at ::= `Array.at`;
Array::forEach ::= `Array.forEach`;
Array::push ::= `Array.push`;
Array::<< ::= `Array.push`;

Variable ::= `Variable`;

Block ::= `Block`;
Block::whileDo ::= `Block.whileDo`;

Fun ::= `Fun`;
Fun::classmethod ::= `Fun.classmethod`;
Fun::staticmethod ::= `Fun.staticmethod`;
Fun::toString ::= `Fun.toString`;

Void ::= `Void`;
Void::toString ::= (this) : {"void";};
void ::= `void`;

Nothing ::= `Nothing`;
Nothing::toString ::= (this) : {"null";};
null ::= `null`;
)::::";

const std::unordered_map<std::string, std::string> STD_MODULES = {
	{":std", std_module}
};

