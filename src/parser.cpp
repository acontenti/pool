#include "parser.hpp"
#include "callable_private.hpp"
#include "pool_private.hpp"
#include <poolstd.hpp>

using namespace pool;
using namespace antlr4;

class ParserImpl : public pool::Parser {
	Location tptl(Token *start, Token *end = nullptr) {
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

	shared_ptr<Callable> parseIdentifier(Token *ast) {
		const auto &id = ast->getText();
		return ParseIdentifier::create(tptl(ast), id);
	}

	shared_ptr<AssignmentAccess> parseAccess(PoolParser::AccessContext *ast) {
		switch (ast->type) {
			case PoolParser::AccessContext::G:
			case PoolParser::AccessContext::L: {
				const auto &caller = parseCall(ast->callee);
				const auto &id = ast->ID()->getText();
				return AssignmentAccess::create(tptl(ast->start, ast->stop), caller, id,
												ast->type == PoolParser::AccessContext::L);
			}
			case PoolParser::AccessContext::I: {
				const auto &id = ast->ID()->getText();
				return AssignmentAccess::create(tptl(ast->start, ast->stop), nullptr, id, false);
			}
			default:
				throw compile_error("invalid access", tptl(ast->start, ast->stop));
		}
	}

	shared_ptr<Callable> parseAssignment(PoolParser::AssignmentContext *ast) {
		switch (ast->type) {
			case PoolParser::AssignmentContext::V: {
				auto assignee = parseAccess(ast->assignee);
				auto value = parseCall(ast->value);
				return Assignment::create(tptl(ast->start, ast->stop), assignee, value, false);
			}
			case PoolParser::AssignmentContext::C: {
				auto assignee = parseAccess(ast->assignee);
				auto value = parseCall(ast->value);
				return Assignment::create(tptl(ast->start, ast->stop), assignee, value, true);
			}
			default:
				throw compile_error("invalid assignment", tptl(ast->start, ast->stop));
		}
	}

	shared_ptr<Callable> parseExpression(PoolParser::ExpressionContext *ast) {
		if (ast->a) {
			return parseAssignment(ast->a);
		} else if (ast->c) {
			return parseCall(ast->c);
		} else throw compile_error("invalid expression", tptl(ast->start, ast->stop));
	}

	vector<shared_ptr<Callable>> parseStatements(const vector<PoolParser::StatementContext *> &statements) {
		vector<shared_ptr<Callable>> result;
		result.reserve(statements.size());
		for (const auto &statement: statements) {
			if (const auto &exp = statement->expression()) {
				result.emplace_back(parseExpression(exp));
			} else {
				result.emplace_back(Identity::create(tptl(statement->start, statement->stop), Null));
			}
		}
		return result;
	}

	shared_ptr<Args> parseArgs(const vector<PoolParser::ArgContext *> &ast) {
		vector<Args::arg_t> args;
		args.reserve(ast.size());
		for (const auto &arg: ast) {
			const auto &ptr = parseCall(arg->call());
			if (arg->DOTS()) {
				args.emplace_back(Expansion::create(ptr));
			} else args.emplace_back(ptr);
		}
		return Args::create(args);
	}

	shared_ptr<Callable> parsePar(PoolParser::ParContext *ast) {
		if (const auto &exp = ast->expression()) {
			return parseExpression(exp);
		} else {
			return Identity::create(tptl(ast->start, ast->stop), Null);
		}
	}

	shared_ptr<Callable> parseNum(PoolParser::NumContext *ast) {
		switch (ast->type) {
			case PoolParser::NumContext::DEC: {
				const auto &token = ast->DECIMAL_INTEGER_LITERAL()->getSymbol();
				return ParseNumber::create(tptl(token), token->getText(), 10);
			}
			case PoolParser::NumContext::HEX: {
				const auto &token = ast->HEX_INTEGER_LITERAL()->getSymbol();
				return ParseNumber::create(tptl(token), token->getText().substr(2), 16);
			}
			case PoolParser::NumContext::BIN: {
				const auto &token = ast->BIN_INTEGER_LITERAL()->getSymbol();
				return ParseNumber::create(tptl(token), token->getText().substr(2), 2);
			}
			case PoolParser::NumContext::FLT: {
				const auto &token = ast->FLOAT_LITERAL()->getSymbol();
				return ParseNumber::create(tptl(token), token->getText(), 0);
			}
			default:
				throw compile_error("invalid number literal", tptl(ast->start, ast->stop));
		}
	}

	shared_ptr<Callable> parseString(PoolParser::StringContext *ast) {
		const auto &token = ast->STRING_LITERAL()->getSymbol();
		const auto &text = token->getText();
		return ParseString::create(tptl(token), text.substr(1, text.size() - 2));
	}

	shared_ptr<Callable> parseBlock(PoolParser::BlockContext *ast) {
		const auto &statements = parseStatements(ast->statement());
		return ParseFunction::create(tptl(ast->start, ast->stop), {}, statements);
	}

	shared_ptr<Callable> parseArray(PoolParser::ArrayContext *ast) {
		const auto &args = parseArgs(ast->arg());
		return ParseArray::create(tptl(ast->start, ast->stop), args);
	}

	shared_ptr<Callable> parseFunction(PoolParser::FunContext *ast) {
		auto paramsAst = ast->param();
		vector<ParseFunction::ParamInfo> params;
		params.reserve(paramsAst.size());
		for (const auto &param: paramsAst) {
			params.push_back(ParseFunction::ParamInfo{
					param->name->getText(),
					tptl(param->name),
					param->type ? param->type->getText() : string(),
					tptl(param->type),
					param->DOTS() != nullptr
			});
		}
		const auto &statements = parseStatements(ast->statement());
		return ParseFunction::create(tptl(ast->start, ast->stop), params, statements);
	}

	shared_ptr<Callable> parseNative(tree::TerminalNode *ast) {
		const auto &token = ast->getSymbol();
		const auto &text = token->getText();
		const auto &id = text.substr(1, text.length() - 2);
		return ParseNative::create(tptl(token), id);
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
				throw compile_error("invalid term", tptl(ast->start, ast->stop));
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
				return Invocation::create(tptl(ast->start, ast->stop), caller, args);
			}
			case PoolParser::CallContext::IA:
			case PoolParser::CallContext::ILA: {
				const auto &caller = parseCall(ast->callee);
				const auto &id = ast->ID()->getText();
				if (ast->a) {
					const auto &args = parseArgs(ast->a->arg());
					return InvocationAccess::create(tptl(ast->start, ast->stop), tptl(ast->ID()->getSymbol()),
													caller, id, args, ast->type == PoolParser::CallContext::ILA);
				} else {
					return Access::create(tptl(ast->start, ast->stop), caller, id,
										  ast->type == PoolParser::CallContext::ILA);
				}
			}
			default:
				throw compile_error("invalid call", tptl(ast->start, ast->stop));
		}
	}

public:
	explicit ParserImpl(const shared_ptr<PoolInstanceImpl> &poolInstance) : Parser(poolInstance) {}

	vector<shared_ptr<Callable>> parseProgram(PoolParser::ProgramContext *ast) override {
		return parseStatements(ast->statement());
	}
};

shared_ptr<pool::Parser> pool::Parser::create(const shared_ptr<PoolInstanceImpl> &poolInstance) {
	return make_shared<ParserImpl>(poolInstance);
}
