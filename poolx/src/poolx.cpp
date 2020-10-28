#include <iostream>
#include "../../include/poolparser.h"
#include "poolx.hpp"

using namespace pool;

const shared_ptr<poolx::Empty> poolx::Void = make_shared<poolx::Empty>();
const shared_ptr<poolx::Nothing> poolx::Null = make_shared<poolx::Nothing>();
const shared_ptr<poolx::Tuple> poolx::Tuple::Empty = make_shared<poolx::Tuple>(vector<shared_ptr<Object>>(), Context::global);
long poolx::Object::ID_COUNTER = 0;
const shared_ptr<poolx::Block> poolx::Block::Empty = make_shared<poolx::Block>(vector<string>(), vector<shared_ptr<poolx::Call>>(), Context::global);
const shared_ptr<poolx::Call> poolx::Call::Empty = make_shared<poolx::Call>(poolx::Block::Empty, "->", poolx::Void, Context::global);
const shared_ptr<poolx::Context> poolx::Context::global = shared_ptr<poolx::Context>(new Context);

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
				calls.push_back(reinterpret_pointer_cast<Call>(term));
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

void poolx::initNatives() {
	auto IO = make_shared<Object>(Context::global);
	IO->addMethod("println", [](const shared_ptr<Object> &other) -> shared_ptr<Object> {
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
	Context::global->add("stdout", make_shared<Variable>("stdout", IO, Context::global));
	Context::global->add("Object", make_shared<Variable>("Object", make_shared<Object>(Context::global), Context::global));
}
