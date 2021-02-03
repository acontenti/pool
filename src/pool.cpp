#include <iostream>
#include <filesystem>
#include <chrono>
#include "poolstd.hpp"
#include "pool.hpp"
#include "../lib/termcolor.hpp"
#include "ErrorListener.hpp"

using namespace pool;
using namespace chrono;
namespace fs = filesystem;

shared_ptr<Fun> parseProgram(PoolParser::ProgramContext *ast, const vector<string> &params, const shared_ptr<Context> &context);

shared_ptr<Callable> parseCall(PoolParser::CallContext *ast, const shared_ptr<Context> &context);

std::string getExecutionTime(high_resolution_clock::time_point startTime, high_resolution_clock::time_point endTime) {
	auto execution_time_ms = duration_cast<std::chrono::microseconds>(endTime - startTime).count();
	auto execution_time_sec = duration_cast<std::chrono::seconds>(endTime - startTime).count();
	auto execution_time_min = duration_cast<std::chrono::minutes>(endTime - startTime).count();
	auto execution_time_hour = duration_cast<std::chrono::hours>(endTime - startTime).count();
	std::stringstream output;
	if (execution_time_hour > 0)
		output << execution_time_hour << "h, ";
	if (execution_time_min > 0)
		output << execution_time_min % 60 << "m, ";
	if (execution_time_sec > 0)
		output << execution_time_sec % 60 << "s, ";
	if (execution_time_ms > 0)
		output << execution_time_ms % long(1E+3) << "ms";
	return output.str();
}

void Pool::setOptions(const Settings &settings) {
	debug = settings.debug;
	for (const auto &arg : settings.args) {
		arguments.push_back(String::create(arg, Context::global));
	}
}

Pool Pool::execute(const string &module) {
	string filename = module;
	if (!endsWith(filename, EXT))
		filename += EXT;
	auto file = fs::path(filename);
	if (fs::exists(file)) {
		if (fs::is_regular_file(file)) {
			if (ifstream(file).good()) {
				file = fs::canonical(file);
				return Pool(file.string());
			} else throw compile_fatal("File \"" + file.string() + "\" is not readable");
		} else throw compile_fatal("File \"" + file.string() + "\" is not a regular file");
	} else throw compile_fatal("File \"" + file.string() + "\" not found");
}

Pool::Pool(const string &file) {
	bool result;
	auto startTime = high_resolution_clock::now();
	ifstream is(file);
	auto inputStream = make_unique<ANTLRInputStream>(is);
	inputStream->name = file;
	auto lexer = make_unique<PoolLexer>(inputStream.get());
	lexer->removeErrorListeners();
	lexer->addErrorListener(ErrorListener::INSTANCE());
	auto tokens = make_unique<CommonTokenStream>(lexer.get());
	auto parser = make_unique<PoolParser>(tokens.get());
	parser->removeErrorListeners();
	parser->addErrorListener(ErrorListener::INSTANCE());
	parser->addParseListener(this);
	try {
		vector<shared_ptr<Callable>> args(arguments.size() + 1);
		args[0] = Identity::create(String::create(file, Context::global));
		transform(arguments.begin(), arguments.end(), args.begin() + 1, [](const shared_ptr<Object> &item) {
			return Identity::create(item);
		});
		auto tree = parser->program();
		if (debug) {
			cout << tree->toStringTree(parser.get()) << endl;
		}
		auto main = parseProgram(tree, {"args"}, Context::global);
		main->execute(nullptr, {Array::create(args, Context::global)->invoke()});
		result = true;
	} catch (::compile_error &error) {
		std::cout << termcolor::red << error << termcolor::reset << std::endl;
		result = false;
	}
	if (debug) {
		auto executionTime = getExecutionTime(startTime, high_resolution_clock::now());
		if (result) {
			std::cout << std::endl << termcolor::green << "Execution successful" << termcolor::reset;
		} else {
			std::cout << std::endl << termcolor::red << "Execution failed" << termcolor::reset;
		}
		std::cout << " in " << executionTime;
		std::cout << std::endl << std::endl;
	}
}

string getId(tree::TerminalNode *ast) {
	return ast->getText().substr(1);
}

shared_ptr<Object> parseIdentifier(tree::TerminalNode *ast, const shared_ptr<Context> &context) {
	auto id = getId(ast);
	if (auto find = context->find(id)) {
		return find;
	} else {
		return context->add(id);
	}
}

shared_ptr<Object> parseNum(PoolParser::NumContext *ast, const shared_ptr<Context> &context) {
	switch (ast->type) {
		case PoolParser::NumContext::DEC: {
			Token *token = ast->DECIMAL_INTEGER_LITERAL()->getSymbol();
			try {
				auto value = stoll(token->getText());
				return Integer::create(value, context);
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
				return Integer::create(value, context);
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
				return Integer::create(value, context);
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
				return Decimal::create(value, context);
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

vector<shared_ptr<Callable>> parseStatements(const vector<PoolParser::StatementContext *> &statements, const shared_ptr<Context> &context) {
	vector<shared_ptr<Callable>> result;
	result.reserve(statements.size());
	for (auto ast : statements) {
		auto call = parseCall(ast->call(), context);
		result.emplace_back(call);
	}
	return result;
}

shared_ptr<Bool> parseBool(PoolParser::BooleanContext *ast, const shared_ptr<Context> &context) {
	return Bool::create(ast->value, context);
}

shared_ptr<String> parseString(PoolParser::StringContext *ast, const shared_ptr<Context> &context) {
	auto value = ast->STRING_LITERAL()->getText();
	value = unescapeString( value.substr(1, value.size() - 2));
	return String::create(value, context);
}

shared_ptr<Array> parseArray(PoolParser::ArrayContext *ast, const shared_ptr<Context> &context) {
	auto callsAst = ast->call();
	vector<shared_ptr<Callable>> result;
	result.reserve(callsAst.size());
	for (auto &call : callsAst) {
		result.emplace_back(parseCall(call, context));
	}
	return Array::create(result, context);
}

shared_ptr<Block> parseBlock(PoolParser::BlockContext *ast, const shared_ptr<Context> &parent) {
	auto context = Context::create(parent);
	auto statements = parseStatements(ast->statement(), context);
	return Block::create(statements, context);
}

shared_ptr<Object> parseFunction(PoolParser::FunContext *ast, const shared_ptr<Context> &parent) {
	auto idsAst = ast->IDENTIFIER();
	vector<string> params;
	params.reserve(idsAst.size());
	for (auto id : idsAst) {
		params.emplace_back(getId(id));
	}
	auto context = Context::create(parent);
	auto statements = parseStatements(ast->statement(), context);
	return Fun::create(params, statements, context);
}

shared_ptr<Callable> parseTerm(PoolParser::TermContext *ast, const shared_ptr<Context> &context) {
	switch (ast->type) {
		case PoolParser::TermContext::NIL:
			return Identity::create(Null);
		case PoolParser::TermContext::NUM:
			return Identity::create(parseNum(ast->num(), context));
		case PoolParser::TermContext::BLN:
			return Identity::create(parseBool(ast->boolean(), context));
		case PoolParser::TermContext::STR:
			return Identity::create(parseString(ast->string(), context));
		case PoolParser::TermContext::FUN:
			return Identity::create(parseFunction(ast->fun(), context));
		case PoolParser::TermContext::ARR:
			return Identity::create(parseArray(ast->array(), context));
		case PoolParser::TermContext::PAR:
			return parseCall(ast->par()->call(), context);
		case PoolParser::TermContext::BLK:
			return Identity::create(parseBlock(ast->block(), context));
		case PoolParser::TermContext::IDT:
			return Identity::create(parseIdentifier(ast->id()->IDENTIFIER(), context));
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

shared_ptr<Callable> parseCall(PoolParser::CallContext *ast, const shared_ptr<Context> &context) {
	if (ast) {
		switch (ast->type) {
			case PoolParser::CallContext::T: {
				auto caller = parseTerm(ast->term(), context);
				return caller;
			}
			case PoolParser::CallContext::TA: {
				auto caller = parseCall(ast->callee, context);
				auto callee = parseArgs(ast->args(), context);
				return Invocation::create(caller, caller, callee);
			}
			case PoolParser::CallContext::TI: {
				auto caller = parseCall(ast->callee, context);
				auto id = getId(ast->IDENTIFIER());
				return Access::create(caller, id);
			}
			case PoolParser::CallContext::TIA: {
				auto caller = parseCall(ast->callee, context);
				auto id = getId(ast->IDENTIFIER());
				auto access = Access::create(caller, id);
				auto callee = parseArgs(ast->args(), context);
				return Invocation::create(caller, access, callee);
			}
			case PoolParser::CallContext::TO: {
				auto caller = parseCall(ast->callee, context);
				auto id = ast->OPERATOR()->getText();
				auto access = Access::create(caller, id);
				return Invocation::create(caller, access, {});
			}
			case PoolParser::CallContext::TOC: {
				auto caller = parseCall(ast->callee, context);
				auto id = ast->OPERATOR()->getText();
				auto callee = parseCall(ast->arg, context);
				auto access = Access::create(caller, id);
				return Invocation::create(caller, access, {callee});
			}
			case PoolParser::CallContext::TOA: {
				auto caller = parseCall(ast->callee, context);
				auto id = ast->OPERATOR()->getText();
				auto access = Access::create(caller, id);
				auto callee = parseArgs(ast->args(), context);
				return Invocation::create(caller, access, callee);
			}
			default:
				throw Pool::compile_fatal("invalid call", ast->getStart());
		}
	} else return Identity::create(Void);
}

shared_ptr<Fun> parseProgram(PoolParser::ProgramContext *ast, const vector<string> &params, const shared_ptr<Context> &context) {
	auto statements = parseStatements(ast->statement(), context);
	return Fun::create(params, statements, context);
}

size_t getNextLineToken(Token *token, Token *original = nullptr) {
	if (!original)
		original = token;
	if (token) {
		if (token->getLine() > original->getLine()) {
			return token->getStartIndex() - token->getCharPositionInLine() - 1;
		} else return getNextLineToken(token->getTokenSource()->nextToken().get(), original);
	} else return original->getStopIndex();
}

void Pool::compile_error(const std::string &message, Token *token, ostream &stream) noexcept {
	return compile_error(message, token, token->getLine(), token->getCharPositionInLine(), stream);
}

void Pool::compile_error(const string &message, Token *token, size_t line, size_t col, ostream &stream) noexcept {
	if (token) {
		stream << token->getInputStream()->getSourceName() << ":" << line << ":" << col + 1 << ": ";
	}
	stream << termcolor::red << termcolor::bold << "ERROR: " << termcolor::reset << message << std::endl;
	if (token) {
		misc::Interval interval(token->getStartIndex() - token->getCharPositionInLine(), getNextLineToken(token));
		auto code = token->getInputStream()->getText(interval);
		code = rtrim(code.substr(0, code.find_first_of('\n')));
		stream << "  " << termcolor::red << code << termcolor::reset << endl;
		stream << "  " << string(token->getCharPositionInLine(), '~') << '^' << endl;
	}
}

::compile_error Pool::compile_fatal(const std::string &message, antlr4::Token *token) noexcept {
	std::stringstream stream;
	compile_error(message, token, stream);
	return ::compile_error(stream.str());
}
