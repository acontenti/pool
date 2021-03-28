#include "poolstd.hpp"
#include "pool.hpp"
#include "util/strings.hpp"
#include "util/errors.hpp"

using namespace pool;

string getId(tree::TerminalNode *ast) {
	return ast->getText();
}

shared_ptr<Object> parseIdentifier(tree::TerminalNode *ast, const shared_ptr<Context> &context) {
	auto id = getId(ast);
	if (auto find = context->find(id)) {
		return find;
	} else {
		return context->add(id);
	}
}

shared_ptr<Callable> parseAccess(PoolParser::AccessContext *ast, const shared_ptr<Context> &context) {
	switch (ast->type) {
		case PoolParser::AccessContext::G: {
			auto caller = parseCall(ast->callee, context);
			auto id = getId(ast->ID());
			return Access::create(ast->getStart(), ast->getStop(), caller, id);
		}
		case PoolParser::AccessContext::L: {
			auto caller = parseCall(ast->callee, context);
			auto id = getId(ast->ID());
			return LocalAccess::create(ast->getStart(), ast->getStop(), caller, id);
		}
		case PoolParser::AccessContext::I: {
			auto id = parseIdentifier(ast->ID(), context);
			return Identity::create(ast->getStart(), ast->getStop(), id);
		}
		default:
			throw compile_error("invalid access", ast->getStart(), ast->getStop());
	}
}

shared_ptr<Callable> parseAssignment(PoolParser::AssignmentContext *ast, const shared_ptr<Context> &context) {
	switch (ast->type) {
		case PoolParser::AssignmentContext::V: {
			auto assignee = parseAccess(ast->assignee, context);
			auto value = parseCall(ast->value, context);
			return Assignment::create(ast->getStart(), ast->getStop(), assignee, value, false);
		}
		case PoolParser::AssignmentContext::C: {
			auto assignee = parseAccess(ast->assignee, context);
			auto value = parseCall(ast->value, context);
			return Assignment::create(ast->getStart(), ast->getStop(), assignee, value, true);
		}
		default:
			throw compile_error("invalid assignment", ast->getStart(), ast->getStop());
	}
}

shared_ptr<Callable> parseExpression(PoolParser::ExpressionContext *ast, const shared_ptr<Context> &context) {
	if (ast->a) {
		return parseAssignment(ast->a, context);
	} else if (ast->c) {
		return parseCall(ast->c, context);
	} else throw compile_error("invalid expression", ast->getStart(), ast->getStop());
}

vector<shared_ptr<Callable>> parseStatements(const vector<PoolParser::StatementContext *> &statements, const shared_ptr<Context> &context) {
	vector<shared_ptr<Callable>> result;
	result.reserve(statements.size());
	for (auto ast : statements) {
		auto exp = ast->expression();
		if (exp) {
			result.emplace_back(parseExpression(exp, context));
		} else {
			result.emplace_back(Identity::create(ast->getStart(), ast->getStop(), Void));
		}
	}
	return result;
}

shared_ptr<Callable> parsePar(PoolParser::ParContext *ast, const shared_ptr<Context> &context) {
	auto exp = ast->expression();
	return exp ? parseExpression(exp, context) : Identity::create(ast->getStart(), ast->getStop(), Void);
}

shared_ptr<Object> parseNum(PoolParser::NumContext *ast, const shared_ptr<Context> &context) {
	switch (ast->type) {
		case PoolParser::NumContext::DEC: {
			Token *token = ast->DECIMAL_INTEGER_LITERAL()->getSymbol();
			try {
				auto value = stoll(token->getText());
				return IntegerClass->newInstance(context, {token, token}, {}, value);
			} catch (const invalid_argument &ex) {
				throw compile_error(ex.what(), token, token);
			} catch (const out_of_range &ex) {
				throw compile_error(ex.what(), token, token);
			}
		}
		case PoolParser::NumContext::HEX: {
			Token *token = ast->HEX_INTEGER_LITERAL()->getSymbol();
			try {
				auto value = stoll(token->getText().substr(2), nullptr, 16);
				return IntegerClass->newInstance(context, {token, token}, {}, value);
			} catch (const invalid_argument &ex) {
				throw compile_error(ex.what(), token, token);
			} catch (const out_of_range &ex) {
				throw compile_error(ex.what(), token, token);
			}
		}
		case PoolParser::NumContext::BIN: {
			Token *token = ast->BIN_INTEGER_LITERAL()->getSymbol();
			try {
				auto value = stoll(token->getText().substr(2), nullptr, 2);
				return IntegerClass->newInstance(context, {token, token}, {}, value);
			} catch (const invalid_argument &ex) {
				throw compile_error(ex.what(), token, token);
			} catch (const out_of_range &ex) {
				throw compile_error(ex.what(), token, token);
			}
		}
		case PoolParser::NumContext::FLT: {
			Token *token = ast->FLOAT_LITERAL()->getSymbol();
			try {
				auto value = stod(token->getText());
				return DecimalClass->newInstance(context, {token, token}, {}, value);
			} catch (const invalid_argument &ex) {
				throw compile_error(ex.what(), token, token);
			} catch (const out_of_range &ex) {
				throw compile_error(ex.what(), token, token);
			}
		}
		default:
			throw compile_error("invalid number literal", ast->getStart(), ast->getStop());
	}
}

shared_ptr<Object> parseString(PoolParser::StringContext *ast, const shared_ptr<Context> &context) {
	auto value = ast->STRING_LITERAL()->getText();
	value = unescapeString(value.substr(1, value.size() - 2));
	return StringClass->newInstance(context, {ast->start, ast->stop}, {}, value);
}

shared_ptr<Object> parseBlock(PoolParser::BlockContext *ast, const shared_ptr<Context> &parent) {
	auto context = Context::create(parent);
	return BlockClass->newInstance(context, {ast->start,
											 ast->stop}, {}, parseStatements(ast->statement(), context), nullptr, nullptr, false);
}

shared_ptr<Object> parseFunction(PoolParser::FunContext *ast, const shared_ptr<Context> &parent) {
	auto paramsAst = ast->param();
	vector<Executable::Param> params;
	params.reserve(paramsAst.size());
	for (auto param = paramsAst.begin(); param != paramsAst.end(); ++param) {
		auto id = getId((*param)->ID());
		auto find = find_if(params.begin(), params.end(), [id](const Executable::Param &p) { return p.id == id; });
		if (find != params.end()) {
			auto idToken = (*param)->ID()->getSymbol();
			throw compile_error("Duplicate parameter '" + id + "'", idToken, idToken);
		}
		if ((*param)->DOTS()) {
			if (next(param) != paramsAst.end()) {
				auto idToken = (*param)->ID()->getSymbol();
				throw compile_error("Variadic parameter '" + id +
									"' must be the last parameter in the function definition", idToken, idToken);
			} else {
				params.emplace_back(id, true);
			}
		} else {
			params.emplace_back(id, false);
		}
	}
	auto context = Context::create(parent);
	for (auto &param : params) {
		context->add(param.id);
	}
	return FunClass->newInstance(context, {ast->start,
										   ast->stop}, {}, params, parseStatements(ast->statement(), context), nullptr, false);
}

shared_ptr<Callable> parseNative(tree::TerminalNode *ast, const shared_ptr<Context> &context) {
	auto id = ast->getText().substr(1, ast->getText().length() - 2);
	auto iterator = natives.find(id);
	if (iterator != natives.end()) {
		return Identity::create(ast->getSymbol(), ast->getSymbol(), iterator->second);
	} else throw compile_error("Native symbol not found", ast->getSymbol(), ast->getSymbol());
}

shared_ptr<Callable> parseTerm(PoolParser::TermContext *ast, const shared_ptr<Context> &context) {
	switch (ast->type) {
		case PoolParser::TermContext::NUM:
			return Identity::create(ast->getStart(), ast->getStop(), parseNum(ast->num(), context));
		case PoolParser::TermContext::STR:
			return Identity::create(ast->getStart(), ast->getStop(), parseString(ast->string(), context));
		case PoolParser::TermContext::FUN:
			return Identity::create(ast->getStart(), ast->getStop(), parseFunction(ast->fun(), context));
		case PoolParser::TermContext::PAR:
			return parsePar(ast->par(), context);
		case PoolParser::TermContext::BLK:
			return Identity::create(ast->getStart(), ast->getStop(), parseBlock(ast->block(), context));
		case PoolParser::TermContext::NSM:
			return parseNative(ast->NATIVE_SYMBOL(), context);
		case PoolParser::TermContext::IDT:
			return Identity::create(ast->getStart(), ast->getStop(), parseIdentifier(ast->ID(), context));
		default:
			throw compile_error("invalid term", ast->getStart(), ast->getStop());
	}
}

shared_ptr<Args> parseArgs(PoolParser::ArgsContext *ast, const shared_ptr<Context> &context) {
	auto argsAst = ast->arg();
	vector<shared_ptr<Callable>> args;
	shared_ptr<Expansion> rest = nullptr;
	args.reserve(argsAst.size());
	for (auto arg : argsAst) {
		auto ptr = parseCall(arg->call(), context);
		if (arg->DOTS()) {
			rest = Expansion::create(ptr);
		} else args.emplace_back(ptr);
	}
	return Args::create(args, rest);
}

shared_ptr<Callable> pool::parseCall(PoolParser::CallContext *ast, const shared_ptr<Context> &context) {
	switch (ast->type) {
		case PoolParser::CallContext::T: {
			return parseTerm(ast->term(), context);
		}
		case PoolParser::CallContext::A: {
			auto caller = parseCall(ast->callee, context);
			auto args = parseArgs(ast->a, context);
			return Invocation::create(ast->getStart(), ast->getStop(), caller, args);
		}
		case PoolParser::CallContext::IA: {
			auto caller = parseCall(ast->callee, context);
			auto id = getId(ast->ID());
			if (ast->a) {
				auto args = parseArgs(ast->a, context);
				return InvocationAccess::create(ast->getStart(), ast->getStop(), ast->ID()->getSymbol(), caller, id, args);
			} else {
				return Access::create(ast->getStart(), ast->getStop(), caller, id);
			}
		}
		default:
			throw compile_error("invalid call", ast->getStart(), ast->getStop());
	}
}

void pool::parseProgram(PoolParser::ProgramContext *ast, const shared_ptr<Context> &context) {
	for (auto statement : ast->statement()) {
		if (auto exp = statement->expression()) {
			parseExpression(exp, context)->invoke();
		}
	}
}
