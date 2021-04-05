#include "natives.hpp"
#include "pool.hpp"
#include "parser.hpp"
#include "util/strings.hpp"
#include "util/errors.hpp"

using namespace pool;

shared_ptr<Callable> parseCall(PoolParser::CallContext *ast, const shared_ptr<Context> &context);

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
			return Access::create({ast->start, ast->stop}, caller, id);
		}
		case PoolParser::AccessContext::L: {
			auto caller = parseCall(ast->callee, context);
			auto id = getId(ast->ID());
			return LocalAccess::create({ast->start, ast->stop}, caller, id);
		}
		case PoolParser::AccessContext::I: {
			auto id = parseIdentifier(ast->ID(), context);
			return Identity::create({ast->start, ast->stop}, id);
		}
		default:
			throw compile_error("invalid access", {ast->start, ast->stop});
	}
}

shared_ptr<Callable> parseAssignment(PoolParser::AssignmentContext *ast, const shared_ptr<Context> &context) {
	switch (ast->type) {
		case PoolParser::AssignmentContext::V: {
			auto assignee = parseAccess(ast->assignee, context);
			auto value = parseCall(ast->value, context);
			return Assignment::create({ast->start, ast->stop}, assignee, value, false);
		}
		case PoolParser::AssignmentContext::C: {
			auto assignee = parseAccess(ast->assignee, context);
			auto value = parseCall(ast->value, context);
			return Assignment::create({ast->start, ast->stop}, assignee, value, true);
		}
		default:
			throw compile_error("invalid assignment", {ast->start, ast->stop});
	}
}

shared_ptr<Callable> pool::parseExpression(PoolParser::ExpressionContext *ast, const shared_ptr<Context> &context) {
	if (ast->a) {
		return parseAssignment(ast->a, context);
	} else if (ast->c) {
		return parseCall(ast->c, context);
	} else throw compile_error("invalid expression", {ast->start, ast->stop});
}

shared_ptr<Callable> parsePar(PoolParser::ParContext *ast, const shared_ptr<Context> &context) {
	auto exp = ast->expression();
	return exp ? parseExpression(exp, context) : Identity::create({ast->start, ast->stop}, Void);
}

shared_ptr<Object> parseNum(PoolParser::NumContext *ast, const shared_ptr<Context> &context) {
	switch (ast->type) {
		case PoolParser::NumContext::DEC: {
			Token *token = ast->DECIMAL_INTEGER_LITERAL()->getSymbol();
			try {
				auto value = stoll(token->getText());
				return Integer::newInstance(context, {token, token}, value);
			} catch (const invalid_argument &ex) {
				throw compile_error(ex.what(), {token, token});
			} catch (const out_of_range &ex) {
				throw compile_error(ex.what(), {token, token});
			}
		}
		case PoolParser::NumContext::HEX: {
			Token *token = ast->HEX_INTEGER_LITERAL()->getSymbol();
			try {
				auto value = stoll(token->getText().substr(2), nullptr, 16);
				return Integer::newInstance(context, {token, token}, value);
			} catch (const invalid_argument &ex) {
				throw compile_error(ex.what(), {token, token});
			} catch (const out_of_range &ex) {
				throw compile_error(ex.what(), {token, token});
			}
		}
		case PoolParser::NumContext::BIN: {
			Token *token = ast->BIN_INTEGER_LITERAL()->getSymbol();
			try {
				auto value = stoll(token->getText().substr(2), nullptr, 2);
				return Integer::newInstance(context, {token, token}, value);
			} catch (const invalid_argument &ex) {
				throw compile_error(ex.what(), {token, token});
			} catch (const out_of_range &ex) {
				throw compile_error(ex.what(), {token, token});
			}
		}
		case PoolParser::NumContext::FLT: {
			Token *token = ast->FLOAT_LITERAL()->getSymbol();
			try {
				auto value = stod(token->getText());
				return Decimal::newInstance(context, {token, token}, value);
			} catch (const invalid_argument &ex) {
				throw compile_error(ex.what(), {token, token});
			} catch (const out_of_range &ex) {
				throw compile_error(ex.what(), {token, token});
			}
		}
		default:
			throw compile_error("invalid number literal", {ast->start, ast->stop});
	}
}

shared_ptr<Object> parseString(PoolParser::StringContext *ast, const shared_ptr<Context> &context) {
	auto value = ast->STRING_LITERAL()->getText();
	value = unescapeString(value.substr(1, value.size() - 2));
	return String::newInstance(context, {ast->start, ast->stop}, value);
}

shared_ptr<Object> parseBlock(PoolParser::BlockContext *ast, const shared_ptr<Context> &parent) {
	return Block::newInstance(parent, {ast->start, ast->stop}, ast->statement());
}

shared_ptr<Object> parseFunction(PoolParser::FunContext *ast, const shared_ptr<Context> &parent) {
	auto paramsAst = ast->param();
	vector<Fun::Param> params;
	params.reserve(paramsAst.size());
	for (auto param = paramsAst.begin(); param != paramsAst.end(); ++param) {
		auto id = getId((*param)->ID());
		auto find = find_if(params.begin(), params.end(), [id](const Fun::Param &p) { return p.id == id; });
		if (find != params.end()) {
			auto idToken = (*param)->ID()->getSymbol();
			throw compile_error("Duplicate parameter '" + id + "'", {idToken, idToken});
		}
		if ((*param)->DOTS()) {
			if (next(param) != paramsAst.end()) {
				auto idToken = (*param)->ID()->getSymbol();
				throw compile_error("Rest parameter '" + id +
									"' must be the last parameter in the function definition", {idToken, idToken});
			} else {
				params.emplace_back(id, true);
			}
		} else {
			params.emplace_back(id, false);
		}
	}
	return Fun::newInstance(parent, {ast->start, ast->stop}, params, ast->statement());
}

shared_ptr<Callable> parseNative(tree::TerminalNode *ast, const shared_ptr<Context> &context) {
	auto id = ast->getText().substr(1, ast->getText().length() - 2);
	if (auto value = Natives::get().find(id)) {
		return Identity::create({ast->getSymbol(), ast->getSymbol()}, value);
	} else throw compile_error("Native symbol not found", {ast->getSymbol(), ast->getSymbol()});
}

shared_ptr<Callable> parseTerm(PoolParser::TermContext *ast, const shared_ptr<Context> &context) {
	switch (ast->type) {
		case PoolParser::TermContext::NUM:
			return Identity::create({ast->start, ast->stop}, parseNum(ast->num(), context));
		case PoolParser::TermContext::STR:
			return Identity::create({ast->start, ast->stop}, parseString(ast->string(), context));
		case PoolParser::TermContext::FUN:
			return Identity::create({ast->start, ast->stop}, parseFunction(ast->fun(), context));
		case PoolParser::TermContext::PAR:
			return parsePar(ast->par(), context);
		case PoolParser::TermContext::BLK:
			return Identity::create({ast->start, ast->stop}, parseBlock(ast->block(), context));
		case PoolParser::TermContext::NSM:
			return parseNative(ast->NATIVE_SYMBOL(), context);
		case PoolParser::TermContext::IDT:
			return Identity::create({ast->start, ast->stop}, parseIdentifier(ast->ID(), context));
		default:
			throw compile_error("invalid term", {ast->start, ast->stop});
	}
}

shared_ptr<Args> parseArgs(PoolParser::ArgsContext *ast, const shared_ptr<Context> &context) {
	auto argsAst = ast->arg();
	vector<Args::arg_t> args;
	args.reserve(argsAst.size());
	for (auto arg : argsAst) {
		auto ptr = parseCall(arg->call(), context);
		if (arg->DOTS()) {
			args.emplace_back(Expansion::create(ptr));
		} else args.emplace_back(ptr);
	}
	return Args::create(args);
}

shared_ptr<Callable> parseCall(PoolParser::CallContext *ast, const shared_ptr<Context> &context) {
	switch (ast->type) {
		case PoolParser::CallContext::T: {
			return parseTerm(ast->term(), context);
		}
		case PoolParser::CallContext::A: {
			auto caller = parseCall(ast->callee, context);
			auto args = parseArgs(ast->a, context);
			return Invocation::create({ast->start, ast->stop}, caller, args);
		}
		case PoolParser::CallContext::IA: {
			auto caller = parseCall(ast->callee, context);
			auto id = getId(ast->ID());
			if (ast->a) {
				auto args = parseArgs(ast->a, context);
				return InvocationAccess::create({ast->start, ast->stop}, ast->ID()->getSymbol(), caller, id, args);
			} else {
				return Access::create({ast->start, ast->stop}, caller, id);
			}
		}
		case PoolParser::CallContext::ILA: {
			auto caller = parseCall(ast->callee, context);
			auto id = getId(ast->ID());
			if (ast->a) {
				auto args = parseArgs(ast->a, context);
				return InvocationLocalAccess::create({ast->start, ast->stop}, ast->ID()->getSymbol(), caller, id, args);
			} else {
				return LocalAccess::create({ast->start, ast->stop}, caller, id);
			}
		}
		default:
			throw compile_error("invalid call", {ast->start, ast->stop});
	}
}

void pool::parseProgram(PoolParser::ProgramContext *ast, const shared_ptr<Context> &context) {
	for (auto statement : ast->statement()) {
		if (auto exp = statement->expression()) {
			parseExpression(exp, context)->invoke();
		}
	}
}
