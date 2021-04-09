#include "callable_private.hpp"
#include <poolstd.hpp>
#include <util/errors.hpp>

using namespace pool;

shared_ptr<Object> Assignment::invoke() {
	const auto &var = assignee->invoke();
	const auto &val = value->invoke();
	if (!var->isImmutable()) {
		var->setValue(val);
		var->setImmutable(immutable);
		return val;
	} else
		throw compile_error("Cannot assign immutable variable '" + var->name + "'", assignee->location);
}

shared_ptr<Object> Invocation::invoke() {
	auto ptr = caller->invoke();
	if (const auto &executable = dynamic_pointer_cast<Executable>(ptr)) {
		const auto &values = args->invoke();
		return executable->execute(ptr, values, location);
	} else throw compile_error(ptr->getRepr() + " is not executable", caller->location);
}

shared_ptr<Object> InvocationAccess::invoke() {
	const auto &selfPtr = caller->invoke();
	const auto &access = local ? selfPtr->findLocal(id) : selfPtr->find(id);
	if (access) {
		const auto &ptr = access->getValue();
		if (const auto &executable = dynamic_pointer_cast<Executable>(ptr)) {
			const auto &values = args->invoke();
			return executable->execute(selfPtr, values, location);
		} else throw compile_error(ptr->getRepr() + " is not executable", {idToken, idToken});
	} else throw compile_error(Null->getRepr() + " is not executable", {idToken, idToken});
}

shared_ptr<Object> Access::invoke() {
	const auto &ptr = caller->invoke();
	const auto &result = local ? ptr->findLocal(id) : ptr->find(id);
	if (result) {
		return result->getValue();
	} else if (const auto &set = ptr->find("set")) {
		if (const auto &function = dynamic_pointer_cast<Function>(set->getValue())) {
			return function->execute(ptr, {String::newInstance(ptr->context, location, id), Null}, location);
		} else throw compile_error("Cannot call method 'set' on '" + ptr->getRepr() + "'", location);
	} else {
		return ptr->context->add(id)->getValue();
	}
}

shared_ptr<Object> Identity::invoke() {
	return object;
}

vector<shared_ptr<Object>> Args::invoke() {
	vector<shared_ptr<Object>> result;
	for (const arg_t &arg : args) {
		if (holds_alternative<shared_ptr<Callable>>(arg)) {
			result.emplace_back(get<shared_ptr<Callable>>(arg)->invoke());
		} else if (holds_alternative<shared_ptr<Expansion>>(arg)) {
			const auto &expansion = get<shared_ptr<Expansion>>(arg)->invoke();
			for (const auto &argarg : expansion) {
				result.emplace_back(argarg);
			}
		} else
			throw runtime_error("Should not happen (" __FILE__ ":" + to_string(__LINE__) + ")");
	}
	return result;
}

vector<shared_ptr<Object>> Expansion::invoke() {
	vector<shared_ptr<Object>> result;
	auto ptr = caller->invoke();
	if (ptr->instanceOf(ArrayClass)) {
		return ptr->as<Array>()->values;
	} else
		throw compile_error("Cannot expand value of type '" + ptr->getClass()->name + "'", caller->location);
}

shared_ptr<Variable> AssignmentAccess::invoke() {
	if (caller) {
		const auto &ptr = caller->invoke();
		const auto &result = local ? ptr->findLocal(id) : ptr->find(id);
		if (result) {
			return result;
		} else if (const auto &set = ptr->find("set")) {
			if (const auto &function = dynamic_pointer_cast<Function>(set->getValue())) {
				function->execute(ptr, {String::newInstance(ptr->context, location, id), Null}, location);
				return ptr->findLocal(id);
			} else throw compile_error("Cannot call method 'set' on '" + ptr->getRepr() + "'", location);
		} else {
			return ptr->context->add(id);
		}
	} else {
		if (auto find = context->find(id)) {
			return find;
		} else {
			return context->add(id);
		}
	}
}
