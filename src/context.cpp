#include <sstream>
#include "poolstd.hpp"

using namespace pool;

const shared_ptr<Context> Context::global = Context::create(nullptr);

shared_ptr<pool::Object> Context::find(const string &name) const {
	if (name == "=") return natives["="];
	if (name == "=>") return natives["=>"];
	auto iterator = heap.find(name);
	if (iterator != heap.end()) {
		return iterator->second;
	} else if (parent) {
		return parent->find(name);
	} else return nullptr;
}

shared_ptr<pool::Object> Context::findLocal(const string &name) const {
	auto iterator = heap.find(name);
	if (iterator != heap.end()) {
		return iterator->second;
	} else return nullptr;
}

void Context::associate(const vector<string> &params, const vector<shared_ptr<pool::Object>> &args) {
	for (int i = 0; i < params.size(); ++i) {
		auto name = params[i];
		auto value = args[i];
		if (const auto &var = find(name)) {
			if (var->isVariable())
				var->as<Variable>()->setValue(value);
			else {
				set(name, value);
			}
		} else {
			add(name, value);
		}
	}
}

shared_ptr<Object> Context::add(const string &name, const shared_ptr<Object> &var) {
	return heap.try_emplace(name, var).first->second;
}

shared_ptr<Object> Context::add(const string &name) {
	return heap.try_emplace(name, VariableClass->newInstance(shared_from_this(), {}, name)).first->second;
}

void Context::set(const string &name, const shared_ptr<Object> &value) {
	heap[name] = value;
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