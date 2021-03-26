#include "poolstd.hpp"

using namespace pool;

shared_ptr<Object> Invocation::invoke() {
	auto ptr = caller->invoke()->as<Object>();
	if (auto executable = dynamic_pointer_cast<Executable>(ptr)) {
		const auto &values = args->invoke();
		return executable->execute(self->invoke(), values);
	} else throw execution_error(ptr->toString() + " is not executable");
}

shared_ptr<Object> Assignment::invoke() {
	auto var = assignee->invoke();
	if (var->isVariable()) {
		auto val = value->invoke();
		var->as<Variable>()->setValue(val);
		var->as<Variable>()->setImmutable(immutable);
		return val;
	} else throw execution_error(var->toString() + " is not a Variable");
}

shared_ptr<Object> Access::invoke() {
	auto ptr = caller->invoke();
	auto result = ptr->find(id);
	return result ? result : ptr->as<Object>()->context->add(id);
}

shared_ptr<Object> LocalAccess::invoke() {
	auto ptr = caller->invoke();
	auto result = ptr->findLocal(id);
	return result ? result : ptr->as<Object>()->context->add(id);
}

shared_ptr<Object> Identity::invoke() {
	return object;
}

vector<shared_ptr<Object>> Args::invoke() {
	vector<shared_ptr<Object>> result;
	for (const auto &arg : args) {
		result.emplace_back(arg->invoke());
	}
	if (rest) {
		const auto &restVector = rest->invoke();
		for (const auto &arg : restVector) {
			result.emplace_back(arg);
		}
	}
	return result;
}

vector<shared_ptr<Object>> Expansion::invoke() {
	vector<shared_ptr<Object>> result;
	auto ptr = caller->invoke();
	if (ptr->getType() == Array::TYPE) {
		return ptr->as<Array>()->values;
	} else throw execution_error("Cannot expand value of type '" + string(ptr->getType()) + "'");
}
