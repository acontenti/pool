#include <iostream>
#include "../../include/libpool.h"
#include "poolx.hpp"

const shared_ptr<poolx::Empty> poolx::Void = make_shared<poolx::Empty>();
const shared_ptr<poolx::Nothing> poolx::Null = make_shared<poolx::Nothing>();
const shared_ptr<poolx::Tuple> poolx::Tuple::Empty = make_shared<poolx::Tuple>(vector<shared_ptr<Object>>());
long poolx::Object::ID_COUNTER = 0;
const shared_ptr<poolx::Block> poolx::Block::Empty = make_shared<poolx::Block>(vector<shared_ptr<poolx::Call>>());
const shared_ptr<poolx::Fun> poolx::Fun::Empty = make_shared<poolx::Fun>(vector<string>(), poolx::Block::Empty);
const shared_ptr<poolx::Call> poolx::Call::Empty = make_shared<poolx::Call>(poolx::Fun::Empty, "->", poolx::Void);
shared_ptr<poolx::Context> poolx::Context::global = shared_ptr<poolx::Context>(new Context);

void poolx::execute(const string &file) {
	string jsonAstString = libpool::generate_json_ast(file.c_str());
	auto jsonAst = json::parse(jsonAstString);
	cout << jsonAst.dump(4) << endl;
	unique_ptr<poolx> plx(new poolx(jsonAst));
	plx->execute();
}

poolx::poolx(const json &jsonAST) {
	initNatives();
	main = parseBlock(jsonAST.begin().value());
}

void poolx::execute() {
	main->execute(vector<shared_ptr<Object>>());
}

shared_ptr<poolx::Object> poolx::parseTerm(const json &ast, const shared_ptr<Context> &context) {
	auto object = ast.get<json::object_t>();
	auto[key, value] = *object.begin();
	if ("Block" == key) {
		return parseBlock(value, context);
	} else if ("Call" == key) {
		return parseCall(value, context);
	} else if ("Fun" == key) {
		return parseFun(value, context);
	} else if ("Decimal" == key) {
		return parseDecimal(value);
	} else if ("Integer" == key) {
		return parseInteger(value);
	} else if ("Bool" == key) {
		return parseBool(value);
	} else if ("Str" == key) {
		return parseString(value);
	} else if ("Ident" == key) {
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
	vector<shared_ptr<Call>> calls;
	shared_ptr<Context> context;
	if(parent)
		context = make_shared<Context>(parent);
	else
		context = Context::global;
	for (auto &callAst : ast) {
		auto object = callAst.get<json::object_t>();
		auto[key, value] = *object.begin();
		if (key == "Call") {
			auto call = parseCall(value, context);
			calls.push_back(call);
		} else if (key == "Empty") {
			calls.push_back(Call::Empty);
		} else throw runtime_error("invalid block element: " + key);
	}
	return make_shared<Block>(calls);
}

shared_ptr<poolx::Call> poolx::parseCall(const json &ast, const shared_ptr<Context> &context) {
	if (ast.is_object()) {
		auto callerAst = ast["caller"];
		auto methodAst = ast["method"];
		auto calleeAst = ast["callee"];
		const shared_ptr<poolx::Object> &caller = parseTerm(callerAst, context);
		auto method = methodAst.get<string>();
		const shared_ptr<poolx::Object> &callee = parseTerm(calleeAst, context);
		return make_shared<Call>(caller, method, callee);
	} else throw runtime_error("invalid call: " + ast.dump());
}

vector<string> poolx::parseArgs(const json &ast) {
	vector<string> result;
	for (auto &item : ast) {
		if (item.is_object()) {
			auto[key, value] = *(item.get<json::object_t>().begin());
			if (key == "Ident")
				result.push_back(value.get<string>());
		}
	}
	return result;
}

shared_ptr<poolx::Fun> poolx::parseFun(const json &ast, const shared_ptr<Context> &context) {
	if (ast.is_object()) {
		auto argsAst = ast["args"].begin().value();
		auto blockAst = ast["block"].begin().value();
		const vector<string> &args = parseArgs(argsAst);
		const shared_ptr<poolx::Block> &block = parseBlock(blockAst, context);
		return make_shared<Fun>(args, block);
	} else throw runtime_error("invalid function: " + ast.dump());
}

shared_ptr<poolx::Decimal> poolx::parseDecimal(const json &ast) {
	auto value = ast.get<double>();
	return make_shared<Decimal>(value);
}

shared_ptr<poolx::Integer> poolx::parseInteger(const json &ast) {
	auto value = ast.get<int64_t>();
	return make_shared<Integer>(value);
}

shared_ptr<poolx::Bool> poolx::parseBool(const json &ast) {
	auto value = ast.get<bool>();
	return make_shared<Bool>(value);
}

shared_ptr<poolx::String> poolx::parseString(const json &ast) {
	auto value = ast.get<string>();
	return make_shared<String>(value);
}

shared_ptr<poolx::Object> poolx::parseIdent(const json &ast, const shared_ptr<Context> &context) {
	auto id = ast.get<string>();
	if (auto find = context->find(id)) {
		return find;
	} else {
		auto var = make_shared<Variable>(id);
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
	return make_shared<Tuple>(terms);
}

shared_ptr<poolx::Array> poolx::parseArray(const json &ast, const shared_ptr<Context> &context) {
	vector<shared_ptr<Object>> terms;
	for (auto &termAst : ast) {
		auto term = parseTerm(termAst, context);
		terms.push_back(term);
	}
	return make_shared<Array>(terms);
}

void poolx::initNatives() {
	auto IO = make_shared<Object>();
	IO->addMethod("println", [this](const shared_ptr<Object> &other) -> shared_ptr<Object> {
		shared_ptr<Object> var = other;
		if (other->getType() == "Variable") {
			var = reinterpret_pointer_cast<Variable>(other)->value;
		}
		if (var->getType() == "Integer") {
			cout << reinterpret_pointer_cast<Integer>(var)->value << endl;
		} else if (var->getType() == "Decimal") {
			cout << reinterpret_pointer_cast<Decimal>(var)->value << endl;
		} else if (var->getType() == "String") {
			cout << reinterpret_pointer_cast<String>(var)->value << endl;
		} else if (auto method = var->findMethod("toString")) {
			auto result = (*method)(Tuple::Empty);
			if (result->getType() == "String") {
				cout << reinterpret_pointer_cast<String>(result)->value << endl;
			} else {
				cout << var->getType() << "@" << var->getID() << endl;
			}
		} else {
			cout << var->getType() << "@" << var->getID() << endl;
		}
		return Void;
	});
	auto ClassClass = make_shared<Object>();
	ClassClass->addMethod("new", [this](const shared_ptr<Object> &other) -> shared_ptr<Object> {
		if (other->getType() == "Fun")
			return make_shared<Class>(reinterpret_pointer_cast<Fun>(other));
		else return Null;
	});
	Context::global->add("stdout", make_shared<Variable>("stdout", IO));
	Context::global->add("Class", make_shared<Variable>("Class", ClassClass));
}
