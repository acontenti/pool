#pragma once

#include <string>
#include <utility>
#include <vector>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

class poolx {
#define TYPER(TYPE) string getType() override{\
        return #TYPE;\
    }

	struct Object {
		typedef function<shared_ptr<Object>(const shared_ptr<Object> &)> method_t;
		typedef unordered_map<string, method_t> methods_map;
	private:
		methods_map methodsMap;
	public:
		virtual string getType() = 0;

		virtual method_t findMethod(const string &name) {
			return methodsMap.at(name);
		};
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
			methods().emplace("+", [this](const shared_ptr<Object> &other) -> shared_ptr<Object> {
				const auto &integer = static_pointer_cast<Integer>(other);
				if (integer) {
					return make_shared<Integer>(this->value + integer->value);
				} else return Null;
			});
		}
	};

	struct Decimal : public Object {
		double value;

		TYPER(Decimal);

		explicit Decimal(double value) : value(value) {
			methods().emplace("+", [this](const shared_ptr<Object> &other) -> shared_ptr<Object> {
				const auto &integer = static_pointer_cast<Decimal>(other);
				if (integer) {
					return make_shared<Integer>(this->value + integer->value);
				} else return Null;
			});
		}
	};

	struct String : public Object {
		string value;

		TYPER(String);

		explicit String(string value) : value(move(value)) {}
	};

	struct Tuple : public Object {
		vector<shared_ptr<Object>> values;

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
				: caller(move(caller)), method(move(method)), callee(move(callee)) {}

		shared_ptr<Object> invoke() {
			auto iterator = caller->methods().find(method);
			if (iterator != caller->methods().end()) {
				return iterator->second(callee);
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

		methods_map findMethod(string name) override {
			return Object::methods() + value->methods();
		}

		explicit Variable(string name, shared_ptr<Object> value) : Named(move(name)), value(move(value)) {
			Object::methods().emplace("=", [this](const shared_ptr<Object> &other) -> shared_ptr<Object> {
				this->value = other;
				return shared_ptr<Variable>(this);
			});
		};
	};

	struct Undefined : public Named {
		TYPER(Undefined);

		explicit Undefined(string name) : Named(move(name)) {
			methods().emplace("=", [this](const shared_ptr<Object> &other) -> shared_ptr<Object> {
				const shared_ptr<Variable> &var = make_shared<Variable>(this->name, other);
				heap.emplace(this->name, var);
				return var;
			});
		}
	};

	json jsonAST;
	static unordered_map<string, shared_ptr<Variable>> heap;
	const static shared_ptr<Object> Null;

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