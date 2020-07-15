#pragma once

#include <string>
#include <utility>
#include <vector>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

class poolx {
#define TYPER(TYPE) string_view getType() const override{\
        return #TYPE;\
    }

	struct Object : enable_shared_from_this<Object> {
		typedef function<shared_ptr<Object>(const shared_ptr<Object> &)> method_t;
	private:
		static long ID_COUNTER;
		unordered_map<string, method_t> methodsMap;
		const long id = ID_COUNTER++;
	public:
		virtual string_view getType() const = 0;

		virtual const method_t *findMethod(const string &name) const {
			auto iterator = methodsMap.find(name);
			return iterator != methodsMap.end() ? &iterator->second : nullptr;
		};

		void addMethod(const string &name, const method_t &method) {
			methodsMap.emplace(name, method);
		};

		long getID() const {
			return id;
		}
	};

	struct Empty : public Object {
		TYPER(Empty);
	};

	struct Bool : public Object {
		bool value;

		TYPER(Bool);

		explicit Bool(bool value) : value(value) {}
	};

	struct Integer : public Object {
		int64_t value;

		TYPER(Integer);

		explicit Integer(int64_t value) : value(value) {
			addMethod("+", [this](const shared_ptr<Object> &other) -> shared_ptr<Object> {
				shared_ptr<Object> var = other;
				if (other->getType() == "Variable") {
					var = reinterpret_pointer_cast<Variable>(other)->value;
				}
				if (var->getType() == "Integer") {
					return make_shared<Integer>(this->value + reinterpret_pointer_cast<Integer>(var)->value);
				} else if (var->getType() == "Decimal") {
					return make_shared<Decimal>(this->value + reinterpret_pointer_cast<Decimal>(var)->value);
				} else if (var->getType() == "String") {
					return make_shared<String>(to_string(this->value) + reinterpret_pointer_cast<String>(var)->value);
				} else return Void;
			});
		}
	};

	struct Decimal : public Object {
		double value;

		TYPER(Decimal);

		explicit Decimal(double value) : value(value) {
			addMethod("+", [this](const shared_ptr<Object> &other) -> shared_ptr<Object> {
				shared_ptr<Object> var = other;
				if (other->getType() == "Variable") {
					var = reinterpret_pointer_cast<Variable>(other)->value;
				}
				if (var->getType() == "Integer") {
					return make_shared<Decimal>(this->value + reinterpret_pointer_cast<Integer>(var)->value);
				} else if (var->getType() == "Decimal") {
					return make_shared<Decimal>(this->value + reinterpret_pointer_cast<Decimal>(var)->value);
				} else if (var->getType() == "String") {
					return make_shared<String>(to_string(this->value) + reinterpret_pointer_cast<String>(var)->value);
				} else return Void;
			});
		}
	};

	struct String : public Object {
		string value;

		TYPER(String);

		explicit String(string value) : value(move(value)) {
			addMethod("+", [this](const shared_ptr<Object> &other) -> shared_ptr<Object> {
				shared_ptr<Object> var = other;
				if (other->getType() == "Variable") {
					var = reinterpret_pointer_cast<Variable>(other)->value;
				}
				if (var->getType() == "Integer") {
					return make_shared<String>(this->value + to_string(reinterpret_pointer_cast<Integer>(var)->value));
				} else if (var->getType() == "Decimal") {
					return make_shared<String>(this->value + to_string(reinterpret_pointer_cast<Decimal>(var)->value));
				} else if (var->getType() == "String") {
					return make_shared<String>(this->value + reinterpret_pointer_cast<String>(var)->value);
				} else return Void;
			});
		}
	};

	struct Tuple : public Object {
		vector<shared_ptr<Object>> values;
		static const shared_ptr<Tuple> Empty;

		TYPER(Tuple);

		explicit Tuple(vector<shared_ptr<Object>> values) : values(move(values)) {}
	};

	struct Array : public Object {
		vector<shared_ptr<Object>> values;

		TYPER(Array);

		explicit Array(vector<shared_ptr<Object>> values) : values(move(values)) {}
	};

	struct Call : public Object {
		shared_ptr<Object> caller;
		string method;
		shared_ptr<Object> callee;

		TYPER(Call);

		Call(shared_ptr<Object> caller, string method, shared_ptr<Object> callee)
				: caller(std::move(caller)), method(std::move(method)), callee(std::move(callee)) {}

		shared_ptr<Object> invoke() {
			auto first = caller, second = callee;
			if (caller->getType() == "Call") {
				first = reinterpret_pointer_cast<Call>(caller)->invoke();
			}
			if (callee->getType() == "Call") {
				second = reinterpret_pointer_cast<Call>(callee)->invoke();
			}
			auto function = first->findMethod(method);
			if (function) {
				return (*function)(second);
			} else throw runtime_error("method " + method + " not found");
		}
	};

	struct Block : public Object {
		vector<shared_ptr<Call>> calls;

		TYPER(Block);

		explicit Block(vector<shared_ptr<Call>> calls) : calls(move(calls)) {}

		void execute() {
			for (auto &call: calls) {
				call->invoke();
			}
		}
	};

	struct Named : public Object {
		string name;

		explicit Named(string name) : name(move(name)) {};
	};

	struct Class : public Named {
		shared_ptr<Class> super = nullptr;

		TYPER(Class);

		explicit Class(string name) : Named(move(name)) {};

		explicit Class(string name, shared_ptr<Class> super) : Named(move(name)), super(std::move(super)) {};
	};

	struct Variable : public Named {
		shared_ptr<Object> value;

		TYPER(Variable);

		const method_t *findMethod(const string &name) const override {
			return Object::findMethod(name) ?: value->findMethod(name);
		}

		explicit Variable(const string &name) : Variable(name, Void) {};

		Variable(const string &name, shared_ptr<Object> value) : Named(name), value(std::move(value)) {
			addMethod("=", [this](const shared_ptr<Object> &other) -> shared_ptr<Object> {
				this->value = other;
				return shared_from_this();
			});
		};
	};

	json jsonAST;
	static unordered_map<string, shared_ptr<Variable>> heap;
	const static shared_ptr<Object> Void;

	explicit poolx(json jsonAST);

	void execute();

	shared_ptr<poolx::Object> parseTerm(const json &ast);

	shared_ptr<Call> parseCall(const json &ast);

	shared_ptr<poolx::Block> parseBlock(const json &ast);

	shared_ptr<Decimal> parseDecimal(const json &ast);

	shared_ptr<Integer> parseInteger(const json &ast);

	shared_ptr<Bool> parseBool(const json &ast);

	shared_ptr<String> parseString(const json &ast);

	shared_ptr<Tuple> parseTuple(const json &ast);

	shared_ptr<Array> parseArray(const json &ast);

	shared_ptr<Object> parseIdent(const json &ast);

	void initNatives();

public:
	static void execute(const string &file);
};