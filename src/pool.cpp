#include "pool_private.hpp"
#include <natives.hpp>
#include "parser.hpp"
#include "llvm_parser.hpp"
#include "llvm/ObjectEmitter.hpp"
#include <poolstd.hpp>
#include <util/strings.hpp>
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
	lexer = make_unique<PoolLexer>(inputStream.get());
	lexer->removeErrorListeners();
	lexer->addErrorListener(this);
	tokens = make_unique<CommonTokenStream>(lexer.get());
	parser = make_unique<PoolParser>(tokens.get());
	parser->removeErrorListeners();
	parser->addErrorListener(this);
}

shared_ptr<Module> PoolInstanceImpl::execute() {
	const auto &statements = Parser::create(shared_from_this())->parseProgram(parser->program());
	const auto &body = CodeFunction::newInstance(Context::global, Location::UNKNOWN, {{"exported"}}, statements)->as<CodeFunction>();
	const auto &context = body->context;
	context->set("__dir__", String::newInstance(context, Location::UNKNOWN, directory), true);
	context->set("__file__", String::newInstance(context, Location::UNKNOWN, path), true);
	return Module::newInstance(Context::global, Location::UNKNOWN, path, body)->as<Module>();
}

void PoolInstanceImpl::compile() noexcept(false) {
	ObjectEmitter::initialize();
	const auto &llvmParser = LLVMParser::create(shared_from_this());
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

shared_ptr<Module> PoolVMImpl::execute(const string &moduleName) noexcept(false) {
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
	shared_ptr<Module> module;
	if (fs::exists(file)) {
		if (fs::is_regular_file(file)) {
			if (ifstream(file).good()) {
				file = fs::canonical(file);
				filename = file.string();
				auto cwd = fs::current_path();
				fs::current_path(file.parent_path());
				const auto &poolInstance = PoolInstance::load(filename);
				module = poolInstance->execute();
				fs::current_path(cwd);
			} else throw runtime_error("File '" + file.string() + "' is not readable");
		} else throw runtime_error("File '" + file.string() + "' is not a regular file");
	} else throw runtime_error("File '" + file.string() + "' not found");
	return module;
}

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
	PoolVMImpl::get()->llvm_context = LLVMNatives::get()->llvm_context;
	initializeStdLib();
	Context::global->set("__debug__", settings.debug ? True : False, true);
	vector<shared_ptr<Object>> arguments;
	arguments.reserve(settings.args.size());
	for (const auto &arg: settings.args) {
		arguments.push_back(String::newInstance(Context::global, Location::UNKNOWN, arg));
	}
	Context::global->set("__args__", Array::newInstance(Context::global, Location::UNKNOWN, arguments), true);
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
