#include <natives.hpp>
#include "pool.hpp"
#include "poolstd_private.hpp"
#include "callable_private.hpp"
#include <util/strings.hpp>
#include <util/errors.hpp>

using namespace pool;
using namespace antlr4;

shared_ptr<Callable> parseCall(PoolParser::CallContext *ast, const shared_ptr<Context> &context);

shared_ptr<Object> parseIdentifier(Token *ast, const shared_ptr<Context> &context) {
	const auto &id = ast->getText();
	if (auto find = context->find(id)) {
		return find->getValue();
	} else {
		return context->add(id)->getValue();
	}
}

shared_ptr<AssignmentAccess> parseAccess(PoolParser::AccessContext *ast, const shared_ptr<Context> &context) {
	switch (ast->type) {
		case PoolParser::AccessContext::G:
		case PoolParser::AccessContext::L: {
			const auto &caller = parseCall(ast->callee, context);
			const auto &id = ast->ID()->getText();
			return AssignmentAccess::create({ast->start, ast->stop}, context, caller, id,
											ast->type == PoolParser::AccessContext::L);
		}
		case PoolParser::AccessContext::I: {
			const auto &id = ast->ID()->getText();
			return AssignmentAccess::create({ast->start, ast->stop}, context, nullptr, id, false);
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

shared_ptr<Object> parseBlock(PoolParser::BlockContext *ast, const shared_ptr<Context> &context) {
	return Block::newInstance(context, {ast->start, ast->stop}, ast->statement());
}

shared_ptr<Object> parseFunction(PoolParser::FunContext *ast, const shared_ptr<Context> &context) {
	auto paramsAst = ast->param();
	vector<Function::Param> params;
	params.reserve(paramsAst.size());
	for (auto param = paramsAst.begin(); param != paramsAst.end(); ++param) {
		const auto &nameToken = (*param)->name;
		const auto &typeToken = (*param)->type;
		const auto &id = nameToken->getText();
		const auto &find = find_if(params.begin(), params.end(), [id](const Function::Param &p) { return p.id == id; });
		if (find != params.end()) {
			throw compile_error("Duplicate parameter '" + id + "'", {nameToken, nameToken});
		}
		if ((*param)->DOTS()) {
			if (typeToken) {
				throw compile_error("Cannot specify type hint on rest parameter '" + id + "'", {typeToken, typeToken});
			}
			if (next(param) != paramsAst.end()) {
				throw compile_error("Rest parameter '" + id +
									"' must be the last parameter in the function definition", {nameToken, nameToken});
			} else {
				params.emplace_back(id, true);
			}
		} else if (typeToken) {
			auto ptr = parseIdentifier(typeToken, context);
			if (ptr->instanceOf(ClassClass)) {
				params.emplace_back(id, ptr->as<Class>());
			} else throw compile_error("Parameter '" + id + "' type hint must be a Class", {typeToken, typeToken});
		} else {
			params.emplace_back(id);
		}
	}
	return CodeFunction::newInstance(context, {ast->start, ast->stop}, params, ast->statement());
}

shared_ptr<Callable> parseNative(tree::TerminalNode *ast, const shared_ptr<Context> &context) {
	const auto &id = ast->getText().substr(1, ast->getText().length() - 2);
	if (const auto &value = Natives::get().find(id)) {
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
			return Identity::create({ast->start, ast->stop}, parseIdentifier(ast->ID()->getSymbol(), context));
		default:
			throw compile_error("invalid term", {ast->start, ast->stop});
	}
}

shared_ptr<Args> parseArgs(PoolParser::ArgsContext *ast, const shared_ptr<Context> &context) {
	const auto &argsAst = ast->arg();
	vector<Args::arg_t> args;
	args.reserve(argsAst.size());
	for (const auto &arg : argsAst) {
		const auto &ptr = parseCall(arg->call(), context);
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
			const auto &caller = parseCall(ast->callee, context);
			const auto &args = parseArgs(ast->a, context);
			return Invocation::create({ast->start, ast->stop}, caller, args);
		}
		case PoolParser::CallContext::IA:
		case PoolParser::CallContext::ILA: {
			const auto &caller = parseCall(ast->callee, context);
			const auto &id = ast->ID()->getText();
			if (ast->a) {
				const auto &args = parseArgs(ast->a, context);
				return InvocationAccess::create({ast->start, ast->stop}, ast->ID()->getSymbol(), caller, id, args,
												ast->type == PoolParser::CallContext::ILA);
			} else {
				return Access::create({ast->start, ast->stop}, caller, id, true);
			}
		}
		default:
			throw compile_error("invalid call", {ast->start, ast->stop});
	}
}

void pool::parseProgram(PoolParser::ProgramContext *ast, const shared_ptr<Context> &context) {
	for (const auto &statement : ast->statement()) {
		if (const auto &exp = statement->expression()) {
			parseExpression(exp, context)->invoke();
		}
	}
}
