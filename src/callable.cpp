#include <algorithm>
#include "poolstd.hpp"

using namespace pool;

shared_ptr<Object> Invocation::invoke() {
	auto ptr = caller->invoke()->as<Object>();
	if (auto executable = dynamic_pointer_cast<Executable>(ptr)) {
		vector<shared_ptr<Object>> args(callee.size());
		transform(callee.begin(), callee.end(), args.begin(), [](const shared_ptr<Callable> &callable) {
			return callable->invoke();
		});
		return executable->execute(self->invoke(), args);
	} else throw execution_error(ptr->toString() + " is not executable");
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
