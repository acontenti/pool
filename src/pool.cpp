#include <iostream>
#include <filesystem>
#include <chrono>
#include "poolstd.hpp"
#include "pool.hpp"
#include "../lib/termcolor.hpp"
#include "ErrorListener.hpp"
#include "PoolLexer.h"
#include "std.hpp"

using namespace pool;
using namespace chrono;
namespace fs = filesystem;

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

void Pool::initialiaze(const Settings &settings) {
	debug = settings.debug;
	pool::initialize();
	arguments.push_back(StringClass->newInstance(Context::global, {}, string()));
	for (const auto &arg : settings.args) {
		arguments.push_back(StringClass->newInstance(Context::global, {}, arg));
	}
	Context::global->set("args", ArrayClass->newInstance(Context::global, arguments));
}

Pool Pool::execute(const string &module) {
	string filename = module;
	if (startsWith(filename, ":")) {
		auto it = STD_MODULES.find(filename);
		if (it != STD_MODULES.end()) {
			istringstream is(it->second);
			return Pool(filename, is);
		}
	}
	if (!endsWith(filename, EXT))
		filename += EXT;
	auto file = fs::path(filename);
	if (fs::exists(file)) {
		if (fs::is_regular_file(file)) {
			if (ifstream(file).good()) {
				file = fs::canonical(file);
				ifstream is(file);
				auto cwd = fs::current_path();
				fs::current_path(file.parent_path());
				const Pool &pool = Pool(file.string(), is);
				fs::current_path(cwd);
				return pool;
			} else throw compile_fatal("File \"" + file.string() + "\" is not readable");
		} else throw compile_fatal("File \"" + file.string() + "\" is not a regular file");
	} else throw compile_fatal("File \"" + file.string() + "\" not found");
}

Pool::Pool(const string &filename, istream &stream) {
	bool result;
	auto startTime = high_resolution_clock::now();
	auto inputStream = make_unique<ANTLRInputStream>(stream);
	inputStream->name = filename;
	auto lexer = make_unique<PoolLexer>(inputStream.get());
	lexer->removeErrorListeners();
	lexer->addErrorListener(ErrorListener::INSTANCE());
	auto tokens = make_unique<CommonTokenStream>(lexer.get());
	auto parser = make_unique<PoolParser>(tokens.get());
	parser->removeErrorListeners();
	parser->addErrorListener(ErrorListener::INSTANCE());
	try {
		auto tree = parser->program();
		if (debug) {
			cout << tree->toStringTree(parser.get()) << endl;
		}
		const auto &oldArgs = Context::global->find("args");
		arguments[0] = StringClass->newInstance(Context::global, {}, filename);
		Context::global->set("args", ArrayClass->newInstance(Context::global, arguments));
		parseProgram(tree, Context::global);
		Context::global->set("args", oldArgs);
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

size_t getNextLineToken(unique_ptr<Token> &&token, Token *original = nullptr) {
	auto &t = token;
	while (t) {
		if (t->getLine() > original->getLine()) {
			return t->getStartIndex() - t->getCharPositionInLine() - 1;
		} else {
			auto orig = t->getStartIndex();
			t = t->getTokenSource()->nextToken();
			if (t && t->getStartIndex() == orig)
				break;
		}
	}
	return original->getStopIndex();
}

void Pool::compile_error(const std::string &message, Token *token, ostream &stream) noexcept {
	if (token)
		return compile_error(message, token, token->getLine(), token->getCharPositionInLine(), stream);
	else {
		return compile_error(message, token, 0, 0, stream);
	}
}

void Pool::compile_error(const string &message, Token *token, size_t line, size_t col, ostream &stream) noexcept {
	if (token) {
		stream << token->getInputStream()->getSourceName() << ":" << line << ":" << col + 1 << ": ";
	}
	stream << termcolor::red << termcolor::bold << "ERROR: " << termcolor::reset << message << std::endl;
	if (token) {
		misc::Interval interval(token->getStartIndex() -
								token->getCharPositionInLine(), getNextLineToken(token->getTokenSource()->nextToken(), token));
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
