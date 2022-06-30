#include "parser.hpp"
#include "callable_private.hpp"
#include "pool_private.hpp"
#include <poolstd.hpp>

using namespace pool;
using namespace antlr4;

Location tptl(const shared_ptr<PoolInstanceImpl> &poolInstance, Token *start, Token *end = nullptr) {
	if (!start) return Location::UNKNOWN;
	if (!end) end = start;
	return Location(Location::Point{
			start->getLine(),
			start->getCharPositionInLine(),
			start->getStartIndex(),
			start->getStopIndex()
	}, Location::Point{
			end->getLine(),
			end->getCharPositionInLine(),
			end->getStartIndex(),
			end->getStopIndex()
	}, poolInstance);
}

shared_ptr<Callable> parseCall(PoolParser::CallContext *ast, const shared_ptr<PoolInstanceImpl> &poolInstance);

shared_ptr<Callable> parseIdentifier(Token *ast, const shared_ptr<PoolInstanceImpl> &poolInstance) {
	const auto &id = ast->getText();
	return ParseIdentifier::create(tptl(poolInstance, ast), id);
}

shared_ptr<AssignmentAccess> parseAccess(PoolParser::AccessContext *ast, const shared_ptr<PoolInstanceImpl> &poolInstance) {
	switch (ast->type) {
		case PoolParser::AccessContext::G:
		case PoolParser::AccessContext::L: {
			const auto &caller = parseCall(ast->callee, poolInstance);
			const auto &id = ast->ID()->getText();
			return AssignmentAccess::create(tptl(poolInstance, ast->start, ast->stop), caller, id,
											ast->type == PoolParser::AccessContext::L);
		}
		case PoolParser::AccessContext::I: {
			const auto &id = ast->ID()->getText();
			return AssignmentAccess::create(tptl(poolInstance, ast->start, ast->stop), nullptr, id, false);
		}
		default:
			throw compile_error("invalid access", tptl(poolInstance, ast->start, ast->stop));
	}
}

shared_ptr<Callable> parseAssignment(PoolParser::AssignmentContext *ast, const shared_ptr<PoolInstanceImpl> &poolInstance) {
	switch (ast->type) {
		case PoolParser::AssignmentContext::V: {
			auto assignee = parseAccess(ast->assignee, poolInstance);
			auto value = parseCall(ast->value, poolInstance);
			return Assignment::create(tptl(poolInstance, ast->start, ast->stop), assignee, value, false);
		}
		case PoolParser::AssignmentContext::C: {
			auto assignee = parseAccess(ast->assignee, poolInstance);
			auto value = parseCall(ast->value, poolInstance);
			return Assignment::create(tptl(poolInstance, ast->start, ast->stop), assignee, value, true);
		}
		default:
			throw compile_error("invalid assignment", tptl(poolInstance, ast->start, ast->stop));
	}
}

shared_ptr<Callable> parseExpression(PoolParser::ExpressionContext *ast, const shared_ptr<PoolInstanceImpl> &poolInstance) {
	if (ast->a) {
		return parseAssignment(ast->a, poolInstance);
	} else if (ast->c) {
		return parseCall(ast->c, poolInstance);
	} else throw compile_error("invalid expression", tptl(poolInstance, ast->start, ast->stop));
}

vector<shared_ptr<Callable>> parseStatements(const vector<PoolParser::StatementContext *> &statements, const shared_ptr<PoolInstanceImpl> &poolInstance) {
	vector<shared_ptr<Callable>> result;
	result.reserve(statements.size());
	for (const auto &statement: statements) {
		if (const auto &exp = statement->expression()) {
			result.emplace_back(parseExpression(exp, poolInstance));
		} else {
			result.emplace_back(Identity::create(tptl(poolInstance, statement->start, statement->stop), Null));
		}
	}
	return result;
}

shared_ptr<Args> parseArgs(const vector<PoolParser::ArgContext *> &ast, const shared_ptr<PoolInstanceImpl> &poolInstance) {
	vector<Args::arg_t> args;
	args.reserve(ast.size());
	for (const auto &arg: ast) {
		const auto &ptr = parseCall(arg->call(), poolInstance);
		if (arg->DOTS()) {
			args.emplace_back(Expansion::create(ptr));
		} else args.emplace_back(ptr);
	}
	return Args::create(args);
}

shared_ptr<Callable> parsePar(PoolParser::ParContext *ast, const shared_ptr<PoolInstanceImpl> &poolInstance) {
	if (const auto &exp = ast->expression()) {
		return parseExpression(exp, poolInstance);
	} else {
		return Identity::create(tptl(poolInstance, ast->start, ast->stop), Null);
	}
}

shared_ptr<Callable> parseNum(PoolParser::NumContext *ast, const shared_ptr<PoolInstanceImpl> &poolInstance) {
	switch (ast->type) {
		case PoolParser::NumContext::DEC: {
			const auto &token = ast->DECIMAL_INTEGER_LITERAL()->getSymbol();
			return ParseNumber::create(tptl(poolInstance, token), token->getText(), 10);
		}
		case PoolParser::NumContext::HEX: {
			const auto &token = ast->HEX_INTEGER_LITERAL()->getSymbol();
			return ParseNumber::create(tptl(poolInstance, token), token->getText().substr(2), 16);
		}
		case PoolParser::NumContext::BIN: {
			const auto &token = ast->BIN_INTEGER_LITERAL()->getSymbol();
			return ParseNumber::create(tptl(poolInstance, token), token->getText().substr(2), 2);
		}
		case PoolParser::NumContext::FLT: {
			const auto &token = ast->FLOAT_LITERAL()->getSymbol();
			return ParseNumber::create(tptl(poolInstance, token), token->getText(), 0);
		}
		default:
			throw compile_error("invalid number literal", tptl(poolInstance, ast->start, ast->stop));
	}
}

shared_ptr<Callable> parseString(PoolParser::StringContext *ast, const shared_ptr<PoolInstanceImpl> &poolInstance) {
	const auto &token = ast->STRING_LITERAL()->getSymbol();
	const auto &text = token->getText();
	return ParseString::create(tptl(poolInstance, token), text.substr(1, text.size() - 2));
}

shared_ptr<Callable> parseBlock(PoolParser::BlockContext *ast, const shared_ptr<PoolInstanceImpl> &poolInstance) {
	const auto &statements = parseStatements(ast->statement(), poolInstance);
	return ParseFunction::create(tptl(poolInstance, ast->start, ast->stop), {}, statements);
}

shared_ptr<Callable> parseArray(PoolParser::ArrayContext *ast, const shared_ptr<PoolInstanceImpl> &poolInstance) {
	const auto &args = parseArgs(ast->arg(), poolInstance);
	return ParseArray::create(tptl(poolInstance, ast->start, ast->stop), args);
}

shared_ptr<Callable> parseFunction(PoolParser::FunContext *ast, const shared_ptr<PoolInstanceImpl> &poolInstance) {
	auto paramsAst = ast->param();
	vector<ParseFunction::ParamInfo> params;
	params.reserve(paramsAst.size());
	for (const auto &param: paramsAst) {
		params.push_back(ParseFunction::ParamInfo{
				param->name->getText(),
				tptl(poolInstance, param->name),
				param->type ? param->type->getText() : string(),
				tptl(poolInstance, param->type),
				param->DOTS() != nullptr
		});
	}
	const auto &statements = parseStatements(ast->statement(), poolInstance);
	return ParseFunction::create(tptl(poolInstance, ast->start, ast->stop), params, statements);
}

shared_ptr<Callable> parseNative(tree::TerminalNode *ast, const shared_ptr<PoolInstanceImpl> &poolInstance) {
	const auto &token = ast->getSymbol();
	const auto &text = token->getText();
	const auto &id = text.substr(1, text.length() - 2);
	return ParseNative::create(tptl(poolInstance, token), id);
}

shared_ptr<Callable> parseTerm(PoolParser::TermContext *ast, const shared_ptr<PoolInstanceImpl> &poolInstance) {
	switch (ast->type) {
		case PoolParser::TermContext::NUM:
			return parseNum(ast->num(), poolInstance);
		case PoolParser::TermContext::STR:
			return parseString(ast->string(), poolInstance);
		case PoolParser::TermContext::FUN:
			return parseFunction(ast->fun(), poolInstance);
		case PoolParser::TermContext::PAR:
			return parsePar(ast->par(), poolInstance);
		case PoolParser::TermContext::BLK:
			return parseBlock(ast->block(), poolInstance);
		case PoolParser::TermContext::ARR:
			return parseArray(ast->array(), poolInstance);
		case PoolParser::TermContext::NSM:
			return parseNative(ast->NATIVE_SYMBOL(), poolInstance);
		case PoolParser::TermContext::IDT:
			return parseIdentifier(ast->ID()->getSymbol(), poolInstance);
		default:
			throw compile_error("invalid term", tptl(poolInstance, ast->start, ast->stop));
	}
}

shared_ptr<Callable> parseCall(PoolParser::CallContext *ast, const shared_ptr<PoolInstanceImpl> &poolInstance) {
	switch (ast->type) {
		case PoolParser::CallContext::T: {
			return parseTerm(ast->term(), poolInstance);
		}
		case PoolParser::CallContext::A: {
			const auto &caller = parseCall(ast->callee, poolInstance);
			const auto &args = parseArgs(ast->a->arg(), poolInstance);
			return Invocation::create(tptl(poolInstance, ast->start, ast->stop), caller, args);
		}
		case PoolParser::CallContext::IA:
		case PoolParser::CallContext::ILA: {
			const auto &caller = parseCall(ast->callee, poolInstance);
			const auto &id = ast->ID()->getText();
			if (ast->a) {
				const auto &args = parseArgs(ast->a->arg(), poolInstance);
				return InvocationAccess::create(tptl(poolInstance, ast->start, ast->stop), tptl(poolInstance, ast->ID()->getSymbol()),
												caller, id, args, ast->type == PoolParser::CallContext::ILA);
			} else {
				return Access::create(tptl(poolInstance, ast->start, ast->stop), caller, id,
									  ast->type == PoolParser::CallContext::ILA);
			}
		}
		default:
			throw compile_error("invalid call", tptl(poolInstance, ast->start, ast->stop));
	}
}

vector<shared_ptr<Callable>> pool::parseProgram(PoolParser::ProgramContext *ast, const shared_ptr<PoolInstanceImpl> &poolInstance) {
	return parseStatements(ast->statement(), poolInstance);
}
