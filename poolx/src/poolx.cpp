#include <iostream>
#include "../../include/poolparser.h"
#include "poolx.hpp"

using namespace pool;

long poolx::Object::ID_COUNTER = 0;
const shared_ptr<poolx::Context> poolx::Context::global = shared_ptr<poolx::Context>(new Context);
const shared_ptr<poolx::Class> poolx::Class::ClassClass = make_shared<Class>("Class", ObjectClass);
const shared_ptr<poolx::Class> poolx::Object::ObjectClass = make_shared<Class>("Object", nullptr);
const shared_ptr<poolx::Class> poolx::Empty::EmptyClass = make_shared<Class>("Empty", ObjectClass);
const shared_ptr<poolx::Class> poolx::Nothing::NothingClass = make_shared<Class>("Nothing", ObjectClass);
const shared_ptr<poolx::Class> poolx::Bool::BoolClass = make_shared<Class>("Bool", ObjectClass);
const shared_ptr<poolx::Class> poolx::Integer::IntegerClass = make_shared<Class>("Integer", ObjectClass);
const shared_ptr<poolx::Class> poolx::Decimal::DecimalClass = make_shared<Class>("Decimal", ObjectClass);
const shared_ptr<poolx::Class> poolx::String::StringClass = make_shared<Class>("String", ObjectClass);
const shared_ptr<poolx::Class> poolx::Tuple::TupleClass = make_shared<Class>("Tuple", ObjectClass);
const shared_ptr<poolx::Class> poolx::Array::ArrayClass = make_shared<Class>("Array", ObjectClass);
const shared_ptr<poolx::Class> poolx::Call::CallClass = make_shared<Class>("Call", ObjectClass);
const shared_ptr<poolx::Class> poolx::Call::Identity::IdentityClass = make_shared<Class>("Identity", ObjectClass);
const shared_ptr<poolx::Class> poolx::Block::BlockClass = make_shared<Class>("Block", ObjectClass);
const shared_ptr<poolx::Empty> poolx::Void = make_shared<poolx::Empty>();
const shared_ptr<poolx::Nothing> poolx::Null = make_shared<poolx::Nothing>();
const shared_ptr<poolx::Tuple> poolx::Tuple::Empty = make_shared<poolx::Tuple>(vector<shared_ptr<Object>>(), Context::global);
const shared_ptr<poolx::Block> poolx::Block::Empty = make_shared<poolx::Block>(vector<string>(), vector<shared_ptr<poolx::Call>>(), Context::global);
const shared_ptr<poolx::Call> poolx::Call::Empty = make_shared<poolx::Call>(poolx::Block::Empty, "->", poolx::Void, Context::global);

shared_ptr<poolx> poolx::load(const string &file, bool debug) {
	string jsonAstString = parser::generate_json_ast(file.c_str());
	auto jsonAst = json::parse(jsonAstString);
	if (debug) {
		cout << jsonAst.dump(4) << endl;
	}
	struct make_shared_enabler : public poolx {
		explicit make_shared_enabler(const json &jsonAst) : poolx(jsonAst) {}
	};
	return make_shared<make_shared_enabler>(jsonAst);
}

poolx::poolx(const json &jsonAST) {
	initNatives();
	main = parseBlock(jsonAST.begin().value());
}

void poolx::execute(const vector<string> &args) {
	vector<shared_ptr<Object>> transformedArgs;
	for (int i = 0; i < args.size(); ++i) {
		main->params.emplace_back("$" + to_string(i));
		transformedArgs.push_back(make_shared<String>(args[i], Context::global));
	}
	main->execute(transformedArgs);
}

shared_ptr<poolx::Object> poolx::parseTerm(const json &ast, const shared_ptr<Context> &context) {
	auto object = ast.get<json::object_t>();
	auto[key, value] = *object.begin();
	if ("Block" == key) {
		return parseBlock(value, context);
	} else if ("Call" == key) {
		return parseCall(value, context);
	} else if ("Decimal" == key) {
		return parseDecimal(value, context);
	} else if ("Integer" == key) {
		return parseInteger(value, context);
	} else if ("Bool" == key) {
		return parseBool(value, context);
	} else if ("Str" == key) {
		return parseString(value, context);
	} else if ("Identifier" == key) {
		return parseIdent(value, context);
	} else if ("Tuple" == key) {
		return parseTuple(value, context);
	} else if ("Array" == key) {
		return parseArray(value, context);
	} else if ("Empty" == key) {
		return Void;
	} else if ("Null" == key) {
		return Null;
	} else {
		throw runtime_error("unknown term type: " + key);
	}
}

shared_ptr<poolx::Block> poolx::parseBlock(const json &ast, const shared_ptr<Context> &parent) {
	auto context = parent ? make_shared<Context>(parent) : Context::global;
	if (ast.is_object()) {
		vector<string> params;
		vector<shared_ptr<Call>> calls;
		auto paramsAst = ast["params"];
		for (auto &item : paramsAst) {
			params.push_back(item.get<string>());
		}
		auto callsAst = ast["calls"];
		for (auto &callAst : callsAst) {
			auto term = parseTerm(callAst, context);
			if (term->getType() == "Call") {
				calls.push_back(static_pointer_cast<Call>(term));
			} else if (term->getType() == "Empty") {
				calls.push_back(Call::Empty);
			} else {
				calls.push_back(Call::Identity(term, context));
			}
		}
		return make_shared<Block>(params, calls, context);
	} else throw runtime_error("invalid block: " + ast.dump());
}

shared_ptr<poolx::Call> poolx::parseCall(const json &ast, const shared_ptr<Context> &context) {
	if (ast.is_object()) {
		auto callerAst = ast["caller"];
		auto methodAst = ast["method"];
		auto calleeAst = ast["callee"];
		const shared_ptr<poolx::Object> &caller = parseTerm(callerAst, context);
		auto method = methodAst.get<string>();
		const shared_ptr<poolx::Object> &callee = parseTerm(calleeAst, context);
		return make_shared<Call>(caller, method, callee, context);
	} else throw runtime_error("invalid call: " + ast.dump());
}

shared_ptr<poolx::Decimal> poolx::parseDecimal(const json &ast, const shared_ptr<Context> &context) {
	auto value = ast.get<double>();
	return make_shared<Decimal>(value, context);
}

shared_ptr<poolx::Integer> poolx::parseInteger(const json &ast, const shared_ptr<Context> &context) {
	auto value = ast.get<int64_t>();
	return make_shared<Integer>(value, context);
}

shared_ptr<poolx::Bool> poolx::parseBool(const json &ast, const shared_ptr<Context> &context) {
	auto value = ast.get<bool>();
	return make_shared<Bool>(value, context);
}

shared_ptr<poolx::String> poolx::parseString(const json &ast, const shared_ptr<Context> &context) {
	auto value = ast.get<string>();
	return make_shared<String>(value, context);
}

shared_ptr<poolx::Variable> poolx::parseIdent(const json &ast, const shared_ptr<Context> &context) {
	auto id = ast.get<string>();
	if (auto find = context->find(id)) {
		return find;
	} else {
		auto var = make_shared<Variable>(id, context);
		context->add(id, var);
		return var;
	}
}

shared_ptr<poolx::Tuple> poolx::parseTuple(const json &ast, const shared_ptr<Context> &context) {
	vector<shared_ptr<Object>> terms;
	for (auto &termAst : ast) {
		auto term = parseTerm(termAst, context);
		terms.push_back(term);
	}
	return make_shared<Tuple>(terms, context);
}

shared_ptr<poolx::Array> poolx::parseArray(const json &ast, const shared_ptr<Context> &context) {
	vector<shared_ptr<Object>> terms;
	for (auto &termAst : ast) {
		auto term = parseTerm(termAst, context);
		terms.push_back(term);
	}
	return make_shared<Array>(terms, context);
}

void poolx::initNatives() {
	Object::ObjectClass->addMethod("toString", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) {
		return make_shared<String>(self->toString(), self->context);
	});
	Object::ObjectClass->addMethod("extend", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		if (other->getType() == "String") {
			shared_ptr<Object> var = self;
			if (self->getType() == "Variable") {
				var = self->as<Variable>()->value;
			}
			return make_shared<Class>(other->as<String>()->value, var->as<Class>(), var->context);
		} else return Null;
	});
	Object::ObjectClass->addMethod("new", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		shared_ptr<Object> var = self;
		if (self->getType() == "Variable") {
			var = self->as<Variable>()->value;
		}
		return make_shared<Object>(var->context, var->as<Class>());
	});
	Empty::EmptyClass->addMethod("toString", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		return make_shared<String>("void", self->context);
	});
	Nothing::NothingClass->addMethod("toString", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		return make_shared<String>("null", self->context);
	});
	Bool::BoolClass->addMethod("toString", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		return make_shared<String>(self->as<Bool>()->value ? "true" : "false", self->context);
	});
	Integer::IntegerClass->addMethod("+", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		shared_ptr<Object> var = other;
		auto value = self->as<Integer>()->value;
		if (other->getType() == "Variable") {
			var = other->as<Variable>()->value;
		}
		if (var->getType() == "Integer") {
			return make_shared<Integer>(value + var->as<Integer>()->value, self->context);
		} else if (var->getType() == "Decimal") {
			return make_shared<Decimal>(value + var->as<Decimal>()->value, self->context);
		} else if (var->getType() == "String") {
			return make_shared<String>(to_string(value) + var->as<String>()->value, self->context);
		} else return Null;
	});
	Integer::IntegerClass->addMethod("toString", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		return make_shared<String>(to_string(self->as<Integer>()->value), self->context);
	});
	Decimal::DecimalClass->addMethod("+", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		shared_ptr<Object> var = other;
		auto value = self->as<Decimal>()->value;
		if (other->getType() == "Variable") {
			var = other->as<Variable>()->value;
		}
		if (var->getType() == "Integer") {
			return make_shared<Decimal>(value + var->as<Integer>()->value, self->context);
		} else if (var->getType() == "Decimal") {
			return make_shared<Decimal>(value + var->as<Decimal>()->value, self->context);
		} else if (var->getType() == "String") {
			return make_shared<String>(to_string(value) + var->as<String>()->value, self->context);
		} else return Null;
	});
	Decimal::DecimalClass->addMethod("toString", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		return make_shared<String>(to_string(self->as<Decimal>()->value), self->context);
	});
	String::StringClass->addMethod("+", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		shared_ptr<Object> var = other;
		if (other->getType() == "Variable") {
			var = other->as<Variable>()->value;
		}
		if (auto method = var->cls->findMethod("toString")) {
			auto result = (*method)(var, Tuple::Empty);
			if (result->getType() == "String") {
				return make_shared<String>(self->as<String>()->value + result->as<String>()->value, self->context);
			}
		}
		return Null;
	});
	String::StringClass->addMethod("toString", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		return self->shared_from_this();
	});
	Call::Identity::IdentityClass->addMethod("", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		return self->as<struct Call::Identity>()->result;
	});
	Block::BlockClass->addMethod("toString", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		stringstream ss;
		ss << "{";
		for (auto &item : *self->context) {
			shared_ptr<Object> var = item.second->value;
			if (var->getType() == "Variable") {
				var = other->as<Variable>()->value;
			}
			if (auto method = var->cls->findMethod("toString")) {
				auto result = (*method)(var, Tuple::Empty);
				ss << item.first << ":";
				if (result->getType() == "String") {
					ss << result->as<String>()->value;
				} else {
					ss << result;
				}
				ss << ",";
			}
		}
		ss.seekp(-1, stringstream::cur); //remove last comma
		ss << "}";
		return make_shared<String>(ss.str(), self->context);
	});
	Block::BlockClass->addMethod("->", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		vector<shared_ptr<Object>> args;
		if (other->getType() == "Empty");
		else if (other->getType() == "Tuple") {
			args = other->as<Tuple>()->values;
		} else {
			args.push_back(other);
		}
		if (self->as<Block>()->params.size() == args.size()) {
			return self->as<Block>()->execute(args);
		} else return Null;
	});
	auto IoClass = make_shared<Class>("Io", Object::ObjectClass);
	IoClass->addMethod("println", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
		shared_ptr<Object> var = other;
		if (other->getType() == "Variable") {
			var = other->as<Variable>()->value;
		}
		if (auto method = var->cls->findMethod("toString")) {
			auto result = (*method)(var, Tuple::Empty);
			if (result->getType() == "String") {
				cout << result->as<String>()->value << endl;
			}
		}
		return Void;
	});
	const shared_ptr<Object> &Io = make_shared<Object>(Context::global, IoClass);
	Context::global->add("stdout", make_shared<Variable>("stdout", Io, Context::global));
	Context::global->add("Object", make_shared<Variable>("Object", Object::ObjectClass, Context::global));
}
