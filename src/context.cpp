#include <sstream>
#include "poolstd.hpp"

using namespace pool;

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

shared_ptr<Variable> Context::add(const string &name) {
	const auto &variable = make_shared<Variable>(Context::create(shared_from_this()), name, Null, false);
	return heap.try_emplace(name, variable).first->second;
}

void Context::set(const string &name, const shared_ptr<Object> &value, bool immutable) {
	if (const auto &var = this->findLocal(name)) {
		var->as<Variable>()->setValue(value->as<Object>());
	} else {
		heap[name] = make_shared<Variable>(Context::create(shared_from_this()), name, value->as<Object>(), immutable);
	}
}

string Context::toString() const {
	stringstream ss;
	ss << "{";
	if (!heap.empty()) {
		for (auto&[name, value] : *this) {
			ss << name << ":" << value->toString() << ",";
		}
		ss.seekp(-1, stringstream::cur); //remove last comma
	}
	ss << "}";
	return ss.str();
}