#include <iostream>
#include "../../include/libpool.h"
#include "poolx.hpp"

const shared_ptr<poolx::Object> poolx::Null = make_shared<poolx::Empty>();
unordered_map<string, shared_ptr<poolx::Variable>> poolx::heap;

void poolx::execute(const string &file) {
	string jsonAstString = libpool::generate_json_ast(file.c_str());
	auto jsonAst = json::parse(jsonAstString);
	cout << jsonAst.dump(4) << endl;
	unique_ptr<poolx> plx(new poolx(jsonAst));
	plx->execute();
}

poolx::poolx(json jsonAST) : jsonAST(move(jsonAST)) {
	initNatives();
}

void poolx::execute() {
	const shared_ptr<Block> &main = static_pointer_cast<Block>(parseTerm(jsonAST));
	main->execute();
}

shared_ptr<poolx::Object> poolx::parseTerm(const json &ast) {
	auto object = ast.get<json::object_t>();
	auto[key, value] = *object.begin();
	if ("Block" == key) {
		return parseBlock(value);
	}
	if ("Call" == key) {
		return parseCall(value);
	}
	if ("Decimal" == key) {
		return parseDecimal(value);
	}
	if ("Integer" == key) {
		return parseInteger(value);
	}
	if ("Bool" == key) {
		return parseBool(value);
	}
	if ("String" == key) {
		return parseString(value);
	}
	if ("Ident" == key) {
		return parseIdent(value);
	}
	if ("Tuple" == key) {
		return parseTuple(value);
	}
	if ("Array" == key) {
		return parseArray(value);
	}
	if ("Empty" == key) {
		return make_shared<Empty>();
	} else {
		throw runtime_error("unknown term type: " + key);
	}
}

shared_ptr<poolx::Block> poolx::parseBlock(const json &ast) {
	vector<shared_ptr<Call>> calls;
	for (auto &callAst : ast) {
		auto object = callAst.get<json::object_t>();
		auto[key, value] = *object.begin();
		if (key == "Call") {
			auto call = parseCall(value);
			calls.push_back(call);
		} else throw runtime_error("invalid block element: " + key);
	}
	return make_shared<Block>(calls);
}

shared_ptr<poolx::Call> poolx::parseCall(const json &ast) {
	if (ast.is_object()) {
		auto callerAst = ast["caller"];
		auto methodAst = ast["method"];
		auto calleeAst = ast["callee"];
		const shared_ptr<poolx::Object> &caller = parseTerm(callerAst);
		auto method = methodAst.get<string>();
		const shared_ptr<poolx::Object> &callee = parseTerm(calleeAst);
		return make_shared<Call>(caller, method, callee);
	} else throw runtime_error("invalid call: " + ast.dump());
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

shared_ptr<poolx::Object> poolx::parseIdent(const json &ast) {
	auto id = ast.get<string>();
	auto iterator = heap.find(id);
	if (iterator != heap.end()) {
		return iterator->second;
	} else {
		return make_shared<Undefined>(id);
	}
}

shared_ptr<poolx::Tuple> poolx::parseTuple(const json &ast) {
	vector<shared_ptr<Object>> terms;
	for (auto &termAst : ast) {
		auto term = parseTerm(termAst);
		terms.push_back(term);
	}
	return make_shared<Tuple>(terms);
}

shared_ptr<poolx::Array> poolx::parseArray(const json &ast) {
	vector<shared_ptr<Object>> terms;
	for (auto &termAst : ast) {
		auto term = parseTerm(termAst);
		terms.push_back(term);
	}
	return make_shared<Array>(terms);
}

void poolx::initNatives() {
	auto IO = make_shared<Class>("IO");
	IO->methods().emplace("println", [this](const shared_ptr<Object> &other) -> shared_ptr<Object> {
		const auto &integer = static_pointer_cast<Integer>(other);
		const auto &var = static_pointer_cast<Variable>(other);
		if (var) {
			const auto &integer = static_pointer_cast<Integer>(var->value);
			if (integer) {
				cout << integer->value << endl;
			}
		}
		if (integer) {
			cout << integer->value << endl;
		}
		return make_shared<Empty>();
	});
	heap.emplace("stdout", make_shared<Variable>("stdout", IO));
}
