#include <iostream>
#include <filesystem>
#include "poolstd.hpp"
#include "pool.hpp"
#include "PoolLexer.h"
#include "util/strings.hpp"
#include "util/errors.hpp"
#include "util/ErrorListener.hpp"
#include <cpplocate/cpplocate.h>

using namespace pool;
namespace fs = filesystem;

void Pool::initialiaze(const Settings &settings) {
	debug = settings.debug;
	pool::initialize();
	arguments.push_back(StringClass->newInstance(Context::global, {}, {}, string()));
	for (const auto &arg : settings.args) {
		arguments.push_back(StringClass->newInstance(Context::global, {}, {}, arg));
	}
	Context::global->set("args", ArrayClass->newInstance(Context::global, {}, arguments));
}

Pool Pool::execute(const string &module) {
	string filename = module;
	if (startsWith(module, ":")) {
		filename = (fs::path(cpplocate::getModulePath()) / ".." / "std" / module.substr(1)).string();
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
			} else throw runtime_error("File '" + file.string() + "' is not readable");
		} else throw runtime_error("File '" + file.string() + "' is not a regular file");
	} else throw runtime_error("File '" + file.string() + "' not found");
}

Pool::Pool(const string &filename, istream &stream) {
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
		const auto &oldArgs = Context::global->find("args");
		arguments[0] = StringClass->newInstance(Context::global, {}, {}, filename);
		Context::global->set("args", ArrayClass->newInstance(Context::global, {}, arguments));
		parseProgram(tree, Context::global);
		Context::global->set("args", oldArgs);
		result = true;
	} catch (const compile_error &error) {
		cerr << error;
		result = false;
	}
}