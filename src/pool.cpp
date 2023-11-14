#include "pool_private.hpp"
#include <natives.hpp>
#include "parser.hpp"
#include "ObjectEmitter.hpp"
#include "util/strings.hpp"
#include <cpplocate/cpplocate.h>
#include <filesystem>
#include <fstream>

using namespace pool;
namespace fs = filesystem;

PoolInstance::PoolInstance(const string &path) : path(path), directory(fs::path(path).parent_path().string()) {}

PoolInstanceImpl::PoolInstanceImpl(const string &path) : PoolInstance(path) {
	ifstream is(path);
	inputStream = make_unique<ANTLRInputStream>(is);
	inputStream->name = path;
}

void PoolInstanceImpl::compile() noexcept(false) {
	lexer = make_unique<PoolLexer>(inputStream.get());
	lexer->removeErrorListeners();
	lexer->addErrorListener(this);
	tokens = make_unique<CommonTokenStream>(lexer.get());
	parser = make_unique<PoolParser>(tokens.get());
	parser->removeErrorListeners();
	parser->addErrorListener(this);
	const auto &llvmParser = Parser::create(shared_from_this());
	llvmParser->parseProgram(parser->program());
	const auto &error = ObjectEmitter::emit(llvmParser->module, fs::path(path).replace_extension(".o").string());
	if (!error.empty()) {
		throw compile_error(error, Location::UNKNOWN);
	}
}

POOL_PUBLIC shared_ptr<PoolInstance> PoolInstance::load(const string &path) {
	return make_shared<PoolInstanceImpl>(path);
}

shared_ptr<PoolVMImpl> PoolVMImpl::instance{};

void PoolVMImpl::compile(const string &moduleName) noexcept(false) {
	string filename = moduleName;
	if (startsWith(moduleName, ":")) {
		filename = (fs::path(PoolVM::getSDKPath()) / "std" / moduleName.substr(1)).string();
	}
	auto file = fs::path(filename);
	if (fs::exists(file) && fs::is_directory(file)) {
		filename = (file / "__module__").string();
	}
	if (!endsWith(filename, PoolVM::EXT)) {
		filename += PoolVM::EXT;
	}
	file = fs::path(filename);
	if (fs::exists(file)) {
		if (fs::is_regular_file(file)) {
			if (ifstream(file).good()) {
				file = fs::canonical(file);
				filename = file.string();
				auto cwd = fs::current_path();
				fs::current_path(file.parent_path());
				const auto &poolInstance = PoolInstance::load(filename);
				poolInstance->compile();
				fs::current_path(cwd);
			} else throw runtime_error("File '" + file.string() + "' is not readable");
		} else throw runtime_error("File '" + file.string() + "' is not a regular file");
	} else throw runtime_error("File '" + file.string() + "' not found");
}

POOL_PUBLIC void PoolVM::initialiaze(const Settings &settings) {
	PoolVMImpl::instance = make_shared<PoolVMImpl>();
	ObjectEmitter::initialize();
	Natives::get()->initialize(settings);
}

POOL_PUBLIC shared_ptr<PoolVM> PoolVM::get() {
	return PoolVMImpl::get();
}

POOL_PUBLIC string PoolVM::getSDKPath() {
	return cpplocate::getModulePath();
}

POOL_PUBLIC string PoolVM::getVersion() {
	return POOL_VERSION;
}
