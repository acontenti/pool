#include <iostream>
#include "../../include/libpool.h"
#include "poolx.hpp"

const shared_ptr<poolx::Empty> poolx::Void = make_shared<poolx::Empty>();
const shared_ptr<poolx::Nothing> poolx::Null = make_shared<poolx::Nothing>();
const shared_ptr<poolx::Tuple> poolx::Tuple::Empty = make_shared<poolx::Tuple>(vector<shared_ptr<Object>>(), Context::global);
long poolx::Object::ID_COUNTER = 0;
const shared_ptr<poolx::Block> poolx::Block::Empty = make_shared<poolx::Block>(vector<shared_ptr<poolx::Call>>(), Context::global);
const shared_ptr<poolx::Fun> poolx::Fun::Empty = make_shared<poolx::Fun>(vector<string>(), poolx::Block::Empty, Context::global);
const shared_ptr<poolx::Call> poolx::Call::Empty = make_shared<poolx::Call>(poolx::Fun::Empty, "->", poolx::Void, Context::global);
shared_ptr<poolx::Context> poolx::Context::global = shared_ptr<poolx::Context>(new Context);

void poolx::execute(const string &file, const vector<string> &args) {
	string jsonAstString = libpool::generate_json_ast(file.c_str());
	auto jsonAst = json::parse(jsonAstString);
	cout << jsonAst.dump(4) << endl;
	unique_ptr<poolx> plx(new poolx(jsonAst, args));
	plx->execute(args);
}

poolx::poolx(const json &jsonAST, const vector<string> &args) {
	initNatives(args);
	main = parseBlock(jsonAST.begin().value());
}

void poolx::execute(const vector<string> &args) {
	vector<string> params;
	vector<shared_ptr<Object>> transformedArgs;
	for (int i = 0; i < args.size(); ++i) {
		params.emplace_back("$" + to_string(i));
		transformedArgs.push_back(make_shared<String>(args[i], Context::global));
	}
	main->execute(params, transformedArgs);
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
		return parseDecimal(value, context);
	} else if ("Integer" == key) {
		return parseInteger(value, context);
	} else if ("Bool" == key) {
		return parseBool(value, context);
	} else if ("Str" == key) {
		return parseString(value, context);
	} else if ("Var" == key) {
		return parseVariable(value, context);
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
	auto context = parent ? make_shared<Context>(parent) : Context::global;
	for (auto &callAst : ast) {
		auto term = parseTerm(callAst, context);
		if (term->getType() == "Call") {
			calls.push_back(reinterpret_pointer_cast<Call>(term));
		} else if (term->getType() == "Empty") {
			calls.push_back(Call::Empty);
		} else {
			calls.push_back(Call::Identity(term, context));
		}
	}
	return make_shared<Block>(calls, context);
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
		for (auto &arg : args) {
			context->add(arg, make_shared<Variable>(arg, context));
		}
		const shared_ptr<poolx::Block> &block = parseBlock(blockAst, context);
		return make_shared<Fun>(args, block, context);
	} else throw runtime_error("invalid function: " + ast.dump());
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

shared_ptr<poolx::Call> poolx::createIdentSearchCall(const json &ast, json::const_reverse_iterator iter, const shared_ptr<Call> &other, const shared_ptr<Context> &context) {
	if (iter == ast.rend()) return other;
	else if (distance(iter, ast.rend()) == 1) {
		return make_shared<Call>(parseIdent(*iter, context), ".", other, context);
	} else {
		auto id = iter->get<string>();
		return createIdentSearchCall(ast, ++iter, make_shared<Call>(make_shared<String>(id, context), ".", other, context), context);
	}
}

shared_ptr<poolx::Object> poolx::parseVariable(const json &ast, const shared_ptr<Context> &context) {
	if (!ast.is_array() || ast.empty()) {
		throw runtime_error("Invalid variable statement");
	} else if (ast.size() == 1) {
		return parseIdent(ast[0], context);
	} else if (ast.size() == 2) {
		return make_shared<Call>(parseIdent(ast[0], context), ".", make_shared<String>(ast[1].get<string>(), context), context);
	} else {
		throw runtime_error("multiple . calls not yet implemented");
//		auto iter = ast.rbegin();
//		auto id = iter->get<string>();
//		auto id2 = (++iter)->get<string>();
//		return createIdentSearchCall(ast, ++iter, make_shared<Call>(make_shared<Variable>(id2, context), ".", make_shared<String>(id)), context);
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

void poolx::initNatives(const vector<string> &args) {
	for (int i = 0; i < args.size(); ++i) {
		auto name = "$" + to_string(i);
		Context::global->add(name, make_shared<Variable>(name, Context::global));
	}
	auto IO = make_shared<Object>(Context::global);
	IO->addMethod("println", [this](const shared_ptr<Object> &other) -> shared_ptr<Object> {
		shared_ptr<Object> var = other;
		if (other->getType() == "Variable") {
			var = reinterpret_pointer_cast<Variable>(other)->value;
		}
		if (auto method = var->findMethod("toString")) {
			auto result = (*method)(Tuple::Empty);
			if (result->getType() == "String") {
				cout << reinterpret_pointer_cast<String>(result)->value << endl;
			}
		}
		return Void;
	});
	auto ClassClass = make_shared<Object>(Context::global);
	ClassClass->addMethod("new", [this](const shared_ptr<Object> &other) -> shared_ptr<Object> {
		if (other->getType() == "Fun")
			return make_shared<Class>(reinterpret_pointer_cast<Fun>(other), Context::global);
		else return Null;
	});
	Context::global->add("stdout", make_shared<Variable>("stdout", IO, Context::global));
	Context::global->add("Class", make_shared<Variable>("Class", ClassClass, Context::global));
}
