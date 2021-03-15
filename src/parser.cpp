#include "poolstd.hpp"
#include "pool.hpp"

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
			return Access::create(caller, id);
		}
		case PoolParser::AccessContext::L: {
			auto caller = parseCall(ast->callee, context);
			auto id = getId(ast->ID());
			return LocalAccess::create(caller, id);
		}
		case PoolParser::AccessContext::I: {
			auto id = parseIdentifier(ast->ID(), context);
			return Identity::create(id);
		}
		default:
			throw Pool::compile_fatal("invalid access", ast->getStart());
	}
}

shared_ptr<Callable> parseAssignment(PoolParser::AssignmentContext *ast, const shared_ptr<Context> &context) {
	switch (ast->type) {
		case PoolParser::AssignmentContext::V: {
			auto assignee = parseAccess(ast->assignee, context);
			auto value = parseCall(ast->value, context);
			return Assignment::create(assignee, value, false);
		}
		case PoolParser::AssignmentContext::C: {
			auto assignee = parseAccess(ast->assignee, context);
			auto value = parseCall(ast->value, context);
			return Assignment::create(assignee, value, true);
		}
		default:
			throw Pool::compile_fatal("invalid assignment", ast->getStart());
	}
}

shared_ptr<Callable> parseExpression(PoolParser::ExpressionContext *ast, const shared_ptr<Context> &context) {
	if (ast) {
		if (ast->a) {
			return parseAssignment(ast->a, context);
		} else if (ast->c) {
			return parseCall(ast->c, context);
		} else throw Pool::compile_fatal("invalid expression", ast->getStart());
	} else return Identity::create(Void);
}

vector<shared_ptr<Callable>> parseStatements(const vector<PoolParser::StatementContext *> &statements, const shared_ptr<Context> &context) {
	vector<shared_ptr<Callable>> result;
	result.reserve(statements.size());
	for (auto ast : statements) {
		result.emplace_back(parseExpression(ast->expression(), context));
	}
	return result;
}

shared_ptr<Object> parseNum(PoolParser::NumContext *ast, const shared_ptr<Context> &context) {
	switch (ast->type) {
		case PoolParser::NumContext::DEC: {
			Token *token = ast->DECIMAL_INTEGER_LITERAL()->getSymbol();
			try {
				auto value = stoll(token->getText());
				return IntegerClass->newInstance(context, {}, value);
			} catch (const invalid_argument &ex) {
				throw Pool::compile_fatal(ex.what(), token);
			} catch (const out_of_range &ex) {
				throw Pool::compile_fatal(ex.what(), token);
			}
		}
		case PoolParser::NumContext::HEX: {
			Token *token = ast->HEX_INTEGER_LITERAL()->getSymbol();
			try {
				auto value = stoll(token->getText().substr(2), nullptr, 16);
				return IntegerClass->newInstance(context, {}, value);
			} catch (const invalid_argument &ex) {
				throw Pool::compile_fatal(ex.what(), token);
			} catch (const out_of_range &ex) {
				throw Pool::compile_fatal(ex.what(), token);
			}
		}
		case PoolParser::NumContext::BIN: {
			Token *token = ast->BIN_INTEGER_LITERAL()->getSymbol();
			try {
				auto value = stoll(token->getText().substr(2), nullptr, 2);
				return IntegerClass->newInstance(context, {}, value);
			} catch (const invalid_argument &ex) {
				throw Pool::compile_fatal(ex.what(), token);
			} catch (const out_of_range &ex) {
				throw Pool::compile_fatal(ex.what(), token);
			}
		}
		case PoolParser::NumContext::FLT: {
			Token *token = ast->FLOAT_LITERAL()->getSymbol();
			try {
				auto value = stod(token->getText());
				return DecimalClass->newInstance(context, {}, value);
			} catch (const invalid_argument &ex) {
				throw Pool::compile_fatal(ex.what(), token);
			} catch (const out_of_range &ex) {
				throw Pool::compile_fatal(ex.what(), token);
			}
		}
		default:
			throw execution_error("Invalid execution point");
	}
}

shared_ptr<Object> parseString(PoolParser::StringContext *ast, const shared_ptr<Context> &context) {
	auto value = ast->STRING_LITERAL()->getText();
	value = unescapeString(value.substr(1, value.size() - 2));
	return StringClass->newInstance(context, {}, value);
}

shared_ptr<Object> parseBlock(PoolParser::BlockContext *ast, const shared_ptr<Context> &parent) {
	auto context = Context::create(parent);
	return BlockClass->newInstance(context, {}, parseStatements(ast->statement(), context));
}

shared_ptr<Object> parseFunction(PoolParser::FunContext *ast, const shared_ptr<Context> &parent) {
	auto idsAst = ast->ID();
	vector<string> params;
	params.reserve(idsAst.size());
	for (auto id : idsAst) {
		params.emplace_back(getId(id));
	}
	auto context = Context::create(parent);
	return FunClass->newInstance(context, {}, params, parseStatements(ast->statement(), context));
}

shared_ptr<Callable> parseNative(tree::TerminalNode *ast, const shared_ptr<Context> &context) {
	auto id = ast->getText().substr(1, ast->getText().length() - 2);
	auto iterator = natives.find(id);
	if (iterator != natives.end()) {
		return Identity::create(iterator->second);
	} else throw Pool::compile_fatal("Native symbol not found", ast->getSymbol());
}

shared_ptr<Callable> parseTerm(PoolParser::TermContext *ast, const shared_ptr<Context> &context) {
	switch (ast->type) {
		case PoolParser::TermContext::NUM:
			return Identity::create(parseNum(ast->num(), context));
		case PoolParser::TermContext::STR:
			return Identity::create(parseString(ast->string(), context));
		case PoolParser::TermContext::FUN:
			return Identity::create(parseFunction(ast->fun(), context));
		case PoolParser::TermContext::PAR:
			return parseExpression(ast->par()->expression(), context);
		case PoolParser::TermContext::BLK:
			return Identity::create(parseBlock(ast->block(), context));
		case PoolParser::TermContext::NSM:
			return parseNative(ast->NATIVE_SYMBOL(), context);
		case PoolParser::TermContext::IDT:
			return Identity::create(parseIdentifier(ast->ID(), context));
		default:
			throw execution_error("Invalid execution point");
	}
}

vector<shared_ptr<Callable>> parseArgs(PoolParser::ArgsContext *ast, const shared_ptr<Context> &context) {
	auto callsAst = ast->call();
	vector<shared_ptr<Callable>> args;
	args.reserve(callsAst.size());
	for (auto call : callsAst) {
		args.emplace_back(parseCall(call, context));
	}
	return args;
}

shared_ptr<Callable> pool::parseCall(PoolParser::CallContext *ast, const shared_ptr<Context> &context) {
	switch (ast->type) {
		case PoolParser::CallContext::T: {
			auto caller = parseTerm(ast->term(), context);
			return caller;
		}
		case PoolParser::CallContext::A: {
			auto caller = parseCall(ast->callee, context);
			auto callee = parseArgs(ast->a, context);
			return Invocation::create(caller, caller, callee);
		}
		case PoolParser::CallContext::IA: {
			auto caller = parseCall(ast->callee, context);
			auto id = getId(ast->ID());
			auto access = Access::create(caller, id);
			if (ast->a) {
				auto callee = parseArgs(ast->a, context);
				return Invocation::create(caller, access, callee);
			} else return access;
		}
		default:
			throw Pool::compile_fatal("invalid call", ast->getStart());
	}
}

shared_ptr<Fun> pool::parseProgram(PoolParser::ProgramContext *ast, const vector<string> &params, const shared_ptr<Context> &context) {
	return FunClass->newInstance(context, {}, params, parseStatements(ast->statement(), context))->as<Fun>();
}
