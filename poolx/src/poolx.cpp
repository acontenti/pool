#include <iostream>
#include "../../include/poolparser.h"
#include "poolx.hpp"

using namespace pool;

shared_ptr<PoolX> PoolX::load(const string &file, bool debug) {
	string jsonAstString = parser::generate_json_ast(file.c_str());
	auto jsonAst = json::parse(jsonAstString);
	if (debug) {
		cout << jsonAst.dump(4) << endl;
	}
	return shared_ptr<PoolX>(new PoolX(jsonAst));
}

PoolX::PoolX(const json &jsonAST) {
	main = parseBlock(jsonAST.begin().value());
}

void PoolX::execute(const vector<string> &args) {
	vector<shared_ptr<Object>> transformedArgs;
	for (int i = 0; i < args.size(); ++i) {
		main->params.emplace_back("$" + to_string(i));
		transformedArgs.push_back(make_shared<String>(args[i], Context::global));
	}
	main->execute(transformedArgs);
}

shared_ptr<Object> PoolX::parseTerm(const json &ast, const shared_ptr<Context> &context) {
	auto object = ast.get<json::object_t>();
	auto[key, value] = *object.begin();
	if ("Block" == key) {
		return parseBlock(value, context);
	} else if ("Call" == key) {
		return parseCall(value, context);
	} else if ("Invocation" == key) {
		return parseInvocation(value, context);
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
	} else if ("Void" == key) {
		return Void;
	} else if ("Null" == key) {
		return Null;
	} else {
		throw runtime_error("unknown term type: " + key);
	}
}

shared_ptr<Block> PoolX::parseBlock(const json &ast, const shared_ptr<Context> &parent) {
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
			} else if (term->getType() == "Void") {
				calls.push_back(Call::Empty);
			} else {
				calls.push_back(Call::Identity(term, context));
			}
		}
		return make_shared<Block>(params, calls, context);
	} else throw runtime_error("invalid block: " + ast.dump());
}

shared_ptr<Call> PoolX::parseCall(const json &ast, const shared_ptr<Context> &context) {
	if (ast.is_object()) {
		auto callerAst = ast["caller"];
		auto methodAst = ast["method"];
		auto calleeAst = ast["callee"];
		const shared_ptr<Object> &caller = parseTerm(callerAst, context);
		auto method = methodAst.get<string>();
		const shared_ptr<Object> &callee = parseTerm(calleeAst, context);
		return make_shared<Call>(caller, method, callee, context);
	} else throw runtime_error("invalid call: " + ast.dump());
}

shared_ptr<Call> PoolX::parseInvocation(const json &ast, const shared_ptr<Context> &context) {
	if (ast.is_object()) {
		auto callerAst = ast["caller"];
		auto argsAst = ast["args"];
		const shared_ptr<Object> &caller = parseTerm(callerAst, context);
		vector<shared_ptr<Object>> args;
		for (auto &argAst : argsAst) {
			args.push_back(parseTerm(argAst, context));
		}
		shared_ptr<Object> arg;
		if (args.empty()) {
			arg = Void;
		} else if (args.size() == 1) {
			arg = args.front();
		} else {
			arg = make_shared<Tuple>(args, context);
		}
		return make_shared<Call>(caller, "->", arg, context);
	} else throw runtime_error("invalid invocation: " + ast.dump());
}

shared_ptr<Decimal> PoolX::parseDecimal(const json &ast, const shared_ptr<Context> &context) {
	auto value = ast.get<double>();
	return make_shared<Decimal>(value, context);
}

shared_ptr<Integer> PoolX::parseInteger(const json &ast, const shared_ptr<Context> &context) {
	auto value = ast.get<int64_t>();
	return make_shared<Integer>(value, context);
}

shared_ptr<Bool> PoolX::parseBool(const json &ast, const shared_ptr<Context> &context) {
	auto value = ast.get<bool>();
	return make_shared<Bool>(value, context);
}

shared_ptr<String> PoolX::parseString(const json &ast, const shared_ptr<Context> &context) {
	auto value = ast.get<string>();
	return make_shared<String>(value, context);
}

shared_ptr<Variable> PoolX::parseIdent(const json &ast, const shared_ptr<Context> &context) {
	auto id = ast.get<string>();
	if (auto find = context->find(id)) {
		return find;
	} else {
		auto var = make_shared<Variable>(id, context);
		context->add(id, var);
		return var;
	}
}

shared_ptr<Tuple> PoolX::parseTuple(const json &ast, const shared_ptr<Context> &context) {
	vector<shared_ptr<Object>> terms;
	for (auto &termAst : ast) {
		auto term = parseTerm(termAst, context);
		terms.push_back(term);
	}
	return make_shared<Tuple>(terms, context);
}

shared_ptr<Array> PoolX::parseArray(const json &ast, const shared_ptr<Context> &context) {
	vector<shared_ptr<Object>> terms;
	for (auto &termAst : ast) {
		auto term = parseTerm(termAst, context);
		terms.push_back(term);
	}
	return make_shared<Array>(terms, context);
}
