#include "callable_private.hpp"
#include <poolstd.hpp>
#include <util/errors.hpp>
#include <util/strings.hpp>
#include <natives.hpp>
#include <Token.h>

using namespace pool;

shared_ptr<Object> Assignment::invoke(const shared_ptr<Context> &context) {
	const auto &var = assignee->invoke(context);
	const auto &val = value->invoke(context);
	if (!var->isImmutable()) {
		var->setValue(val);
		var->setImmutable(immutable);
		return val;
	} else
		throw compile_error("Cannot assign immutable variable '" + var->name + "'", assignee->location);
}

shared_ptr<Object> Invocation::invoke(const shared_ptr<Context> &context) {
	auto ptr = caller->invoke(context);
	if (const auto &executable = dynamic_pointer_cast<Executable>(ptr)) {
		const auto &values = args->invoke(context);
		return executable->execute(ptr, values, location);
	} else throw compile_error(ptr->getRepr() + " is not executable", caller->location);
}

shared_ptr<Object> InvocationAccess::invoke(const shared_ptr<Context> &context) {
	const auto &selfPtr = caller->invoke(context);
	const auto &access = local ? selfPtr->findLocal(id) : selfPtr->find(id);
	if (access) {
		const auto &ptr = access->getValue();
		if (const auto &executable = dynamic_pointer_cast<Executable>(ptr)) {
			const auto &values = args->invoke(context);
			return executable->execute(selfPtr, values, location);
		} else throw compile_error(ptr->getRepr() + " is not executable", {idToken, idToken});
	} else throw compile_error(Null->getRepr() + " is not executable", {idToken, idToken});
}

shared_ptr<Object> Access::invoke(const shared_ptr<Context> &context) {
	const auto &ptr = caller->invoke(context);
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

shared_ptr<Object> Identity::invoke(const shared_ptr<Context> &context) {
	return object;
}

vector<shared_ptr<Object>> Args::invoke(const shared_ptr<Context> &context) {
	vector<shared_ptr<Object>> result;
	for (const arg_t &arg : args) {
		if (holds_alternative<shared_ptr<Callable>>(arg)) {
			result.emplace_back(get<shared_ptr<Callable>>(arg)->invoke(context));
		} else if (holds_alternative<shared_ptr<Expansion>>(arg)) {
			const auto &expansion = get<shared_ptr<Expansion>>(arg)->invoke(context);
			for (const auto &argarg : expansion) {
				result.emplace_back(argarg);
			}
		} else
			throw runtime_error("Should not happen (" __FILE__ ":" + to_string(__LINE__) + ")");
	}
	return result;
}

vector<shared_ptr<Object>> Expansion::invoke(const shared_ptr<Context> &context) {
	vector<shared_ptr<Object>> result;
	auto ptr = caller->invoke(context);
	if (ptr->instanceOf(ArrayClass)) {
		return ptr->as<Array>()->values;
	} else
		throw compile_error("Cannot expand value of type '" + ptr->getClass()->name + "'", caller->location);
}

shared_ptr<Variable> AssignmentAccess::invoke(const shared_ptr<Context> &context) {
	if (caller) {
		const auto &ptr = caller->invoke(context);
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

shared_ptr<Object> ParseBlock::invoke(const shared_ptr<Context> &context) {
	return Block::newInstance(context, location, statements);
}

shared_ptr<Object> ParseFunction::invoke(const shared_ptr<Context> &context) {
	vector<Function::Param> params;
	params.reserve(paramInfos.size());
	for (auto paramInfo = paramInfos.begin(); paramInfo != paramInfos.end(); ++paramInfo) {
		const auto &nameToken = paramInfo->name;
		const auto &typeToken = paramInfo->type;
		const auto &id = nameToken->getText();
		const auto &find = find_if(params.begin(), params.end(), [id](const Function::Param &p) { return p.id == id; });
		if (find != params.end()) {
			throw compile_error("Duplicate parameter '" + id + "'", {nameToken, nameToken});
		}
		if (paramInfo->rest) {
			if (typeToken) {
				throw compile_error("Cannot specify type hint on rest parameter '" + id + "'", {typeToken, typeToken});
			}
			if (next(paramInfo) != paramInfos.end()) {
				throw compile_error("Rest parameter '" + id +
									"' must be the last parameter in the function definition", {nameToken, nameToken});
			} else {
				params.emplace_back(id, true);
			}
		} else if (typeToken) {
			const auto &type = typeToken->getText();
			if (const auto &var = context->find(type)) {
				const auto &ptr = var->getValue();
				if (ptr->instanceOf(ClassClass)) {
					params.emplace_back(id, ptr->as<Class>());
				} else throw compile_error("Parameter '" + id + "' type hint must be a Class", {typeToken, typeToken});
			} else throw compile_error("Parameter '" + id + "' type hint is undefined", {typeToken, typeToken});
		} else {
			params.emplace_back(id);
		}
	}
	return CodeFunction::newInstance(context, location, params, statements);
}

shared_ptr<Object> ParseString::invoke(const shared_ptr<Context> &context) {
	const auto &unescaped = unescapeString(value);
	return String::newInstance(context, location, unescaped);
}

shared_ptr<Object> ParseNative::invoke(const shared_ptr<Context> &context) {
	if (const auto &value = Natives::get().find(id)) {
		return value;
	} else throw compile_error("Native symbol '" + id + "' not found", location);
}

shared_ptr<Object> ParseIdentifier::invoke(const shared_ptr<Context> &context) {
	if (const auto &find = context->find(id)) {
		return find->getValue();
	} else {
		return context->add(id)->getValue();
	}
}

shared_ptr<Object> ParseNumber::invoke(const shared_ptr<Context> &context) {
	try {
		if (base == 0) {
			const auto &value = stod(text);
			return Decimal::newInstance(context, location, value);
		} else {
			const auto &value = stoll(text, nullptr, base);
			return Integer::newInstance(context, location, value);
		}
	} catch (const invalid_argument &ex) {
		throw compile_error("Error parsing "s + (base == 0 ? "Decimal" : "Integer") + ": " + ex.what(), location);
	} catch (const out_of_range &ex) {
		throw compile_error("Error parsing "s + (base == 0 ? "Decimal" : "Integer") + ": " + ex.what(), location);
	}
}

shared_ptr<Object> ParseArray::invoke(const shared_ptr<Context> &context) {
	return Array::newInstance(context, location, args->invoke(context));
}
