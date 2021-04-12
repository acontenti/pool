#include "pool_private.hpp"
#include "parser.hpp"
#include <poolstd.hpp>
#include <util/strings.hpp>
#include <cpplocate/cpplocate.h>
#include <filesystem>
#include <fstream>

using namespace pool;
namespace fs = filesystem;

PoolInstance::PoolInstance(string path) : path(move(path)) {}

PoolInstanceImpl::PoolInstanceImpl(const string &path) : PoolInstance(path) {
	ifstream is(path);
	inputStream = make_unique<ANTLRInputStream>(is);
	inputStream->name = path;
	lexer = make_unique<PoolLexer>(inputStream.get());
	lexer->removeErrorListeners();
	lexer->addErrorListener(this);
	tokens = make_unique<CommonTokenStream>(lexer.get());
	parser = make_unique<PoolParser>(tokens.get());
	parser->removeErrorListeners();
	parser->addErrorListener(this);
}

void PoolInstanceImpl::execute() {
	const auto &oldFilename = Context::global->find("file");
	Context::global->forceSet("file", String::newInstance(Context::global, Location::UNKNOWN, path), true);
	const auto &statements = parseProgram(parser->program(), shared_from_this());
	for (const auto &statement :statements) {
		statement->invoke(Context::global);
	}
	if (oldFilename) {
		Context::global->forceSet("file", oldFilename->getValue(), true);
	}
}

shared_ptr<PoolVMImpl> PoolVMImpl::instance{};

void PoolVMImpl::execute(const string &module) noexcept(false) {
	string filename = module;
	if (startsWith(module, ":")) {
		filename = (fs::path(cpplocate::getModulePath()) / "std" / module.substr(1)).string();
	}
	if (!endsWith(filename, PoolVM::EXT))
		filename += PoolVM::EXT;
	auto file = fs::path(filename);
	if (fs::exists(file)) {
		if (fs::is_regular_file(file)) {
			if (ifstream(file).good()) {
				file = fs::canonical(file);
				filename = file.string();
				if (instances.find(filename) == instances.end()) {
					auto cwd = fs::current_path();
					fs::current_path(file.parent_path());
					const auto &poolInstance = make_shared<PoolInstanceImpl>(filename);
					instances[filename] = poolInstance;
					poolInstance->execute();
					fs::current_path(cwd);
				}
			} else throw runtime_error("File '" + file.string() + "' is not readable");
		} else throw runtime_error("File '" + file.string() + "' is not a regular file");
	} else throw runtime_error("File '" + file.string() + "' not found");
}

void PoolVM::initialiaze(const Settings &settings) {
	PoolVMImpl::instance = make_shared<PoolVMImpl>();
	initializeStdLib();
	Context::global->set("debug", settings.debug ? True : False, true);
	vector<shared_ptr<Object>> arguments;
	for (const auto &arg : settings.args) {
		arguments.push_back(String::newInstance(Context::global, Location::UNKNOWN, arg));
	}
	Context::global->set("args", Array::newInstance(Context::global, Location::UNKNOWN, arguments), true);
}

shared_ptr<PoolVM> PoolVM::get() {
	return PoolVMImpl::get();
}
