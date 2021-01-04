#include <poolparser.h>
#include <iostream>
#include <filesystem>
#include <fstream>
#include "poolstd.hpp"
#include "poolx.hpp"
#include "../../lib/json.hpp"

using namespace pool;
using json = nlohmann::json;
namespace fs = filesystem;

shared_ptr<Object> parseTerm(const json &ast, const shared_ptr<Context> &context);

shared_ptr<Call> parseCall(const json &ast, const shared_ptr<Context> &context);

shared_ptr<Call> parseInvocation(const json &ast, const shared_ptr<Context> &context);

shared_ptr<Block> parseBlock(const json &ast, const shared_ptr<Context> &parent);

shared_ptr<Decimal> parseDecimal(const json &ast, const shared_ptr<Context> &context);

shared_ptr<Integer> parseInteger(const json &ast, const shared_ptr<Context> &context);

shared_ptr<Bool> parseBool(const json &ast, const shared_ptr<Context> &context);

shared_ptr<String> parseString(const json &ast, const shared_ptr<Context> &context);

shared_ptr<Tuple> parseTuple(const json &ast, const shared_ptr<Context> &context);

shared_ptr<Array> parseArray(const json &ast, const shared_ptr<Context> &context);

shared_ptr<Variable> parseIdent(const json &ast, const shared_ptr<Context> &context);

void PoolX::setOptions(const Settings &settings) {
	debug = settings.debug;
	for (const auto &arg : settings.args) {
		arguments.push_back(make_shared<String>(arg, Context::global));
	}
}

PoolX PoolX::execute(const string &module) {
	string filename = module;
	if (!endsWith(filename, EXT))
		filename += EXT;
	auto file = fs::path(filename);
	if (fs::exists(file)) {
		if (fs::is_regular_file(file)) {
			if (ifstream(file).good()) {
				file = fs::canonical(file);
				return PoolX(file.string());
			} else throw runtime_error("File \"" + file.string() + "\" is not readable");
		} else throw runtime_error("File \"" + file.string() + "\" is not a regular file");
	} else throw runtime_error("File \"" + file.string() + "\" does not exist");
}

PoolX::PoolX(const string &file) {
	string jsonAstString = parser::generate_json_ast(file.c_str());
	auto jsonAst = json::parse(jsonAstString);
	if (debug) {
		cout << jsonAst.dump(4) << endl;
	}
	auto main = parseBlock(jsonAst.begin().value(), Context::global);
	for (int i = 0; i < arguments.size() + 1; ++i) {
		main->params.emplace_back(to_string(i));
	}
	vector<shared_ptr<Object>> args{make_shared<String>(file, Context::global)};
	args.insert(args.end(), arguments.begin(), arguments.end());
	main->execute(args);
}

shared_ptr<Object> parseTerm(const json &ast, const shared_ptr<Context> &context) {
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

shared_ptr<Block> parseBlock(const json &ast, const shared_ptr<Context> &parent) {
	auto context = make_shared<Context>(parent);
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

shared_ptr<Call> parseCall(const json &ast, const shared_ptr<Context> &context) {
	if (ast.is_object()) {
		auto callerAst = ast["caller"];
		auto methodAst = ast["method"];
		auto calleeAst = ast["callee"];
		const shared_ptr<Object> &caller = parseTerm(callerAst, context);
		auto method = methodAst.get<string>();
		shared_ptr<Object> callee;
		if (method == ".")
			callee = parseTerm(calleeAst, make_shared<Context>(nullptr));
		else
			callee = parseTerm(calleeAst, context);
		if (caller->getType() == "Void") {
			return make_shared<Call>(callee, method, Void, true, context);
		} else {
			return make_shared<Call>(caller, method, callee, false, context);
		}
	} else throw runtime_error("invalid call: " + ast.dump());
}

shared_ptr<Call> parseInvocation(const json &ast, const shared_ptr<Context> &context) {
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
		return make_shared<Call>(caller, "->", arg, false, context);
	} else throw runtime_error("invalid invocation: " + ast.dump());
}

shared_ptr<Decimal> parseDecimal(const json &ast, const shared_ptr<Context> &context) {
	auto value = ast.get<double>();
	return make_shared<Decimal>(value, context);
}

shared_ptr<Integer> parseInteger(const json &ast, const shared_ptr<Context> &context) {
	auto value = ast.get<int64_t>();
	return make_shared<Integer>(value, context);
}

shared_ptr<Bool> parseBool(const json &ast, const shared_ptr<Context> &context) {
	auto value = ast.get<bool>();
	return make_shared<Bool>(value, context);
}

shared_ptr<String> parseString(const json &ast, const shared_ptr<Context> &context) {
	auto value = ast.get<string>();
	return make_shared<String>(value, context);
}

shared_ptr<Variable> parseIdent(const json &ast, const shared_ptr<Context> &context) {
	auto id = ast.get<string>();
	if (auto find = context->find(id)) {
		return find;
	} else {
		auto var = Variable::create(id, context);
		context->add(var);
		return var;
	}
}

shared_ptr<Tuple> parseTuple(const json &ast, const shared_ptr<Context> &context) {
	vector<shared_ptr<Object>> terms;
	for (auto &termAst : ast) {
		auto term = parseTerm(termAst, context);
		terms.push_back(term);
	}
	return make_shared<Tuple>(terms, context);
}

shared_ptr<Array> parseArray(const json &ast, const shared_ptr<Context> &context) {
	vector<shared_ptr<Object>> terms;
	for (auto &termAst : ast) {
		auto term = parseTerm(termAst, context);
		terms.push_back(term);
	}
	return make_shared<Array>(terms, context);
}
