#include "poolstd.hpp"
#include "pool.hpp"
#include "util/errors.hpp"
#include "callable.hpp"


using namespace pool;

shared_ptr<Object> Assignment::invoke() {
	const auto &ptr = assignee->invoke();
	if (ptr->isVariable()) {
		const auto &val = value->invoke();
		const auto &var = ptr->as<Variable>();
		if (!var->isImmutable()) {
			var->setValue(val->as<Object>());
			var->setImmutable(immutable);
			return val;
		} else
			throw compile_error("Cannot assign immutable variable '" + var->name + "'", assignee->location);
	} else throw compile_error(ptr->toString() + " is not a Variable", assignee->location);
}

shared_ptr<Object> Invocation::invoke() {
	auto ptr = caller->invoke()->as<Object>();
	if (auto executable = dynamic_pointer_cast<Executable>(ptr)) {
		const auto &values = args->invoke();
		return executable->execute(ptr, values, location);
	} else throw compile_error(ptr->toString() + " is not executable", caller->location);
}

shared_ptr<Object> InvocationAccess::invoke() {
	const auto &selfPtr = caller->invoke();
	auto access = selfPtr->find(id);
	if (access) {
		const auto &ptr = access->as<Object>();
		if (auto executable = dynamic_pointer_cast<Executable>(ptr)) {
			const auto &values = args->invoke();
			return executable->execute(selfPtr, values, location);
		} else throw compile_error(ptr->toString() + " is not executable", idToken, idToken);
	} else throw compile_error(Null->toString() + " is not executable", idToken, idToken);
}

shared_ptr<Object> InvocationLocalAccess::invoke() {
	const auto &selfPtr = caller->invoke();
	auto access = selfPtr->findLocal(id);
	if (access) {
		const auto &ptr = access->as<Object>();
		if (auto executable = dynamic_pointer_cast<Executable>(ptr)) {
			const auto &values = args->invoke();
			return executable->execute(selfPtr, values, location);
		} else throw compile_error(ptr->toString() + " is not executable", idToken, idToken);
	} else throw compile_error(Null->toString() + " is not executable", idToken, idToken);
}

shared_ptr<Object> Access::invoke() {
	auto ptr = caller->invoke();
	auto result = ptr->find(id);
	if (result) {
		return result;
	} else if (auto set = ptr->find("set")) {
		if (auto exe = dynamic_pointer_cast<Executable>(set->as<Object>())) {
			return exe->execute(ptr, {StringClass->newInstance(ptr->context, location, {}, id), Null}, location);
		} else throw compile_error("Cannot call method 'set' on '" + ptr->toString() + "'", location);
	} else {
		return ptr->as<Object>()->context->add(id);
	}
}

shared_ptr<Object> LocalAccess::invoke() {
	auto ptr = caller->invoke();
	auto result = ptr->findLocal(id);
	if (result) {
		return result;
	} else if (auto set = ptr->find("set")) {
		if (auto exe = dynamic_pointer_cast<Executable>(set->as<Object>())) {
			return exe->execute(ptr, {StringClass->newInstance(ptr->context, location, {}, id), Null}, location);
		} else throw compile_error("Cannot call method 'set' on '" + ptr->toString() + "'", location);
	} else {
		return ptr->as<Object>()->context->add(id);
	}
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
	} else
		throw compile_error("Cannot expand value of type '" + string(ptr->getType()) + "'", caller->location);
}
