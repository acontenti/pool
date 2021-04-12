#include <memory>
#include <sstream>
#include <poolstd.hpp>
#include <util/errors.hpp>

using namespace pool;

Variable::Variable(shared_ptr<Object> value, bool immutable)
		: value(move(value)), immutable(immutable) {
}

void Variable::setValue(const shared_ptr<Object> &val) {
	if (!immutable)
		value = val;
}

bool Variable::isImmutable() const {
	return immutable;
}

void Variable::setImmutable(bool _immutable) {
	immutable = _immutable;
}

const shared_ptr<Object> &Variable::getValue() const {
	return value;
}

shared_ptr<Context> Context::global = nullptr;

shared_ptr<Variable> Context::find(const string &name) const {
	auto iterator = heap.find(name);
	if (iterator != heap.end()) {
		return iterator->second;
	} else if (parent) {
		return parent->find(name);
	} else return nullptr;
}

shared_ptr<Variable> Context::findLocal(const string &name) const {
	auto iterator = heap.find(name);
	if (iterator != heap.end()) {
		return iterator->second;
	} else return nullptr;
}

shared_ptr<Variable> Context::findOrAdd(const string &name, bool local) {
	if (const auto &var = local ? this->findLocal(name) : this->find(name)) {
		return var;
	} else {
		return heap[name] = make_shared<Variable>(Null, false);
	}
}

shared_ptr<Variable> Context::set(const string &name, const shared_ptr<Object> &value, bool immutable, const Location &location) {
	if (const auto &var = this->findLocal(name)) {
		if (!var->isImmutable()) {
			var->setValue(value);
			var->setImmutable(immutable);
		} else throw compile_error("Cannot assign immutable variable '" + name + "'", location);
		return var;
	} else return heap[name] = make_shared<Variable>(value, immutable);
}

shared_ptr<Variable> Context::forceSet(const string &name, const shared_ptr<Object> &value, bool immutable) {
	if (const auto &var = this->findLocal(name)) {
		var->setValue(value);
		var->setImmutable(immutable);
		return var;
	} else return heap[name] = make_shared<Variable>(value, immutable);
}

void Context::remove(const string &name) {
	heap.erase(name);
}

Context::Context(shared_ptr<Context> parent) : parent(move(parent)) {}

shared_ptr<Context> Context::create(const shared_ptr<Context> &parent) {
	return make_shared<Context>(parent);
}
