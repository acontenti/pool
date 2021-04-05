#include <iostream>
#include <filesystem>
#include <memory>
#include "poolstd.hpp"
#include "pool.hpp"
#include "parser.hpp"
#include "PoolLexer.h"
#include "util/strings.hpp"
#include "util/errors.hpp"
#include "util/ErrorListener.hpp"
#include <cpplocate/cpplocate.h>

using namespace pool;
namespace fs = filesystem;

PoolVM::Settings PoolVM::settings;
vector<shared_ptr<PoolVM::PoolInstance>> PoolVM::instances;

class PoolVM::PoolInstance {
public:
	unique_ptr<ANTLRInputStream> inputStream;
	unique_ptr<PoolLexer> lexer;
	unique_ptr<CommonTokenStream> tokens;
	unique_ptr<PoolParser> parser;
	bool result;

	PoolInstance(const string &filename, istream &stream) {
		inputStream = make_unique<ANTLRInputStream>(stream);
		inputStream->name = filename;
		lexer = make_unique<PoolLexer>(inputStream.get());
		lexer->removeErrorListeners();
		lexer->addErrorListener(ErrorListener::INSTANCE());
		tokens = make_unique<CommonTokenStream>(lexer.get());
		parser = make_unique<PoolParser>(tokens.get());
		parser->removeErrorListeners();
		parser->addErrorListener(ErrorListener::INSTANCE());
		try {
			vector<shared_ptr<Object>> arguments;
			arguments.push_back(String::newInstance(Context::global, Location::UNKNOWN, filename));
			for (const auto &arg : settings.args) {
				arguments.push_back(String::newInstance(Context::global, Location::UNKNOWN, arg));
			}
			Context::global->set("debug", settings.debug ? True : False);
			const auto &oldArgs = Context::global->find("args");
			Context::global->set("args", ArrayClass->newInstance(Context::global, Location::UNKNOWN, arguments));
			parseProgram(parser->program(), Context::global);
			if (oldArgs) {
				Context::global->set("args", oldArgs);
			}
			result = true;
		} catch (const compile_error &error) {
			cerr << error;
			result = false;
		}
	}
};

void PoolVM::initialiaze(const Settings &_settings) {
	PoolVM::settings = _settings;
	initializeStdLib();
}

bool PoolVM::execute(const string &module) {
	string filename = module;
	if (startsWith(module, ":")) {
		filename = (fs::path(cpplocate::getModulePath()) / "std" / module.substr(1)).string();
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
				const auto &pool = make_shared<PoolInstance>(file.string(), is);
				instances.emplace_back(pool);
				fs::current_path(cwd);
				return pool->result;
			} else throw runtime_error("File '" + file.string() + "' is not readable");
		} else throw runtime_error("File '" + file.string() + "' is not a regular file");
	} else throw runtime_error("File '" + file.string() + "' not found");
}
