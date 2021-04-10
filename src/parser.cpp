#include "parser.hpp"
#include <poolstd.hpp>
#include "callable_private.hpp"
#include <util/errors.hpp>

using namespace pool;
using namespace antlr4;

shared_ptr<Callable> parseCall(PoolParser::CallContext *ast);

shared_ptr<Callable> parseIdentifier(Token *ast) {
	const auto &id = ast->getText();
	return ParseIdentifier::create({ast, ast}, id);
}

shared_ptr<AssignmentAccess> parseAccess(PoolParser::AccessContext *ast) {
	switch (ast->type) {
		case PoolParser::AccessContext::G:
		case PoolParser::AccessContext::L: {
			const auto &caller = parseCall(ast->callee);
			const auto &id = ast->ID()->getText();
			return AssignmentAccess::create({ast->start, ast->stop}, caller, id,
											ast->type == PoolParser::AccessContext::L);
		}
		case PoolParser::AccessContext::I: {
			const auto &id = ast->ID()->getText();
			return AssignmentAccess::create({ast->start, ast->stop}, nullptr, id, false);
		}
		default:
			throw compile_error("invalid access", {ast->start, ast->stop});
	}
}

shared_ptr<Callable> parseAssignment(PoolParser::AssignmentContext *ast) {
	switch (ast->type) {
		case PoolParser::AssignmentContext::V: {
			auto assignee = parseAccess(ast->assignee);
			auto value = parseCall(ast->value);
			return Assignment::create({ast->start, ast->stop}, assignee, value, false);
		}
		case PoolParser::AssignmentContext::C: {
			auto assignee = parseAccess(ast->assignee);
			auto value = parseCall(ast->value);
			return Assignment::create({ast->start, ast->stop}, assignee, value, true);
		}
		default:
			throw compile_error("invalid assignment", {ast->start, ast->stop});
	}
}

shared_ptr<Callable> parseExpression(PoolParser::ExpressionContext *ast) {
	if (ast->a) {
		return parseAssignment(ast->a);
	} else if (ast->c) {
		return parseCall(ast->c);
	} else throw compile_error("invalid expression", {ast->start, ast->stop});
}

vector<shared_ptr<Callable>> parseStatements(const vector<PoolParser::StatementContext *> &statements) {
	vector<shared_ptr<Callable>> result;
	result.reserve(statements.size());
	for (const auto &statement : statements) {
		const auto &exp = statement->expression();
		result.emplace_back(exp ? parseExpression(exp) : Identity::create({statement->start, statement->stop}, Void));
	}
	return result;
}

shared_ptr<Args> parseArgs(const vector<PoolParser::ArgContext *> &ast) {
	vector<Args::arg_t> args;
	args.reserve(ast.size());
	for (const auto &arg : ast) {
		const auto &ptr = parseCall(arg->call());
		if (arg->DOTS()) {
			args.emplace_back(Expansion::create(ptr));
		} else args.emplace_back(ptr);
	}
	return Args::create(args);
}

shared_ptr<Callable> parsePar(PoolParser::ParContext *ast) {
	const auto &exp = ast->expression();
	return exp ? parseExpression(exp) : Identity::create({ast->start, ast->stop}, Void);
}

shared_ptr<Callable> parseNum(PoolParser::NumContext *ast) {
	switch (ast->type) {
		case PoolParser::NumContext::DEC: {
			const auto &token = ast->DECIMAL_INTEGER_LITERAL()->getSymbol();
			return ParseNumber::create({token, token}, token->getText(), 10);
		}
		case PoolParser::NumContext::HEX: {
			const auto &token = ast->HEX_INTEGER_LITERAL()->getSymbol();
			return ParseNumber::create({token, token}, token->getText().substr(2), 16);
		}
		case PoolParser::NumContext::BIN: {
			const auto &token = ast->BIN_INTEGER_LITERAL()->getSymbol();
			return ParseNumber::create({token, token}, token->getText().substr(2), 2);
		}
		case PoolParser::NumContext::FLT: {
			const auto &token = ast->FLOAT_LITERAL()->getSymbol();
			return ParseNumber::create({token, token}, token->getText(), 0);
		}
		default:
			throw compile_error("invalid number literal", {ast->start, ast->stop});
	}
}

shared_ptr<Callable> parseString(PoolParser::StringContext *ast) {
	const auto &token = ast->STRING_LITERAL()->getSymbol();
	const auto &text = token->getText();
	return ParseString::create({token, token}, text.substr(1, text.size() - 2));
}

shared_ptr<Callable> parseBlock(PoolParser::BlockContext *ast) {
	const auto &statements = parseStatements(ast->statement());
	return ParseBlock::create({ast->start, ast->stop}, statements);
}

shared_ptr<Callable> parseArray(PoolParser::ArrayContext *ast) {
	const auto &args = parseArgs(ast->arg());
	return ParseArray::create({ast->start, ast->stop}, args);
}

shared_ptr<Callable> parseFunction(PoolParser::FunContext *ast) {
	auto paramsAst = ast->param();
	vector<ParseFunction::ParamInfo> params;
	params.reserve(paramsAst.size());
	for (const auto &param : paramsAst) {
		params.push_back({param->name, param->type, param->DOTS() != nullptr});
	}
	return ParseFunction::create({ast->start, ast->stop}, params, parseStatements(ast->statement()));
}

shared_ptr<Callable> parseNative(tree::TerminalNode *ast) {
	const auto &token = ast->getSymbol();
	const auto &text = token->getText();
	const auto &id = text.substr(1, text.length() - 2);
	return ParseNative::create({token, token}, id);
}

shared_ptr<Callable> parseTerm(PoolParser::TermContext *ast) {
	switch (ast->type) {
		case PoolParser::TermContext::NUM:
			return parseNum(ast->num());
		case PoolParser::TermContext::STR:
			return parseString(ast->string());
		case PoolParser::TermContext::FUN:
			return parseFunction(ast->fun());
		case PoolParser::TermContext::PAR:
			return parsePar(ast->par());
		case PoolParser::TermContext::BLK:
			return parseBlock(ast->block());
		case PoolParser::TermContext::ARR:
			return parseArray(ast->array());
		case PoolParser::TermContext::NSM:
			return parseNative(ast->NATIVE_SYMBOL());
		case PoolParser::TermContext::IDT:
			return parseIdentifier(ast->ID()->getSymbol());
		default:
			throw compile_error("invalid term", {ast->start, ast->stop});
	}
}

shared_ptr<Callable> parseCall(PoolParser::CallContext *ast) {
	switch (ast->type) {
		case PoolParser::CallContext::T: {
			return parseTerm(ast->term());
		}
		case PoolParser::CallContext::A: {
			const auto &caller = parseCall(ast->callee);
			const auto &args = parseArgs(ast->a->arg());
			return Invocation::create({ast->start, ast->stop}, caller, args);
		}
		case PoolParser::CallContext::IA:
		case PoolParser::CallContext::ILA: {
			const auto &caller = parseCall(ast->callee);
			const auto &id = ast->ID()->getText();
			if (ast->a) {
				const auto &args = parseArgs(ast->a->arg());
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

vector<shared_ptr<Callable>> pool::parseProgram(PoolParser::ProgramContext *ast) {
	return parseStatements(ast->statement());
}
