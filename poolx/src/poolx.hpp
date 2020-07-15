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
		virtual string_view getType() const {
			return "Object";
		};

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

		Empty() {
			addMethod("toString", [this](const shared_ptr<Object> &other) -> shared_ptr<Object> {
				static auto toString = make_shared<String>("void");
				return toString;
			});
		}
	};

	struct Nothing : public Object {
		TYPER(Nothing);

		Nothing() {
			addMethod("toString", [this](const shared_ptr<Object> &other) -> shared_ptr<Object> {
				static auto toString = make_shared<String>("null");
				return toString;
			});
		}
	};

	struct Bool : public Object {
		bool value;

		TYPER(Bool);

		explicit Bool(bool value) : value(value) {
			addMethod("toString", [this](const shared_ptr<Object> &other) -> shared_ptr<Object> {
				static auto toString = make_shared<String>(this->value ? "true" : "false");
				return toString;
			});
		}
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
				} else return Null;
			});
			addMethod("toString", [this](const shared_ptr<Object> &other) -> shared_ptr<Object> {
				static auto toString = make_shared<String>(to_string(this->value));
				return toString;
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
				} else return Null;
			});
			addMethod("toString", [this](const shared_ptr<Object> &other) -> shared_ptr<Object> {
				static auto toString = make_shared<String>(to_string(this->value));
				return toString;
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
				} else return Null;
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

		static const shared_ptr<Call> Empty;

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

	struct Named : public Object {
		string name;

		explicit Named(string name) : name(move(name)) {};
	};

	struct Variable : public Named {
		shared_ptr<Object> value;

		TYPER(Variable);

		const method_t *findMethod(const string &name) const override {
			return Object::findMethod(name) ?: value->findMethod(name);
		}

		explicit Variable(const string &name) : Variable(name, Null) {};

		Variable(const string &name, shared_ptr<Object> value) : Named(name), value(std::move(value)) {
			addMethod("=", [this](const shared_ptr<Object> &other) -> shared_ptr<Object> {
				shared_ptr<Object> var = other;
				if (other->getType() == "Variable") {
					var = reinterpret_pointer_cast<Variable>(other)->value;
				}
				this->value = var;
				return shared_from_this();
			});
		};
	};

	struct Context {
	private:
		unordered_map<string, shared_ptr<Variable>> heap;
		const shared_ptr<Context> parent;

		Context() : Context(nullptr) {}

	public:
		static shared_ptr<Context> global;

		explicit Context(shared_ptr<Context> parent) : parent(std::move(parent)) {}

		shared_ptr<Variable> find(const string &name) const {
			auto iterator = heap.find(name);
			if (iterator != heap.end()) {
				return iterator->second;
			} else if (parent) {
				return parent->find(name);
			} else return nullptr;
		}

		void add(const string &name, const shared_ptr<Variable> &var) {
			heap.emplace(name, var);
		}

		void associate(const vector<string> &params, const vector<shared_ptr<Object>> &args) {
			for (int i = 0; i < params.size(); ++i) {
				auto name = params[i];
				auto value = args[i];
				if (const auto &var = find(name)) {
					var->value = value;
				} else {
					add(name, make_shared<Variable>(name, value));
				}
			}
		}
	};

	struct Block : public Object {
		vector<shared_ptr<Call>> calls;
		shared_ptr<Context> context;
		static const shared_ptr<Block> Empty;

		TYPER(Block);

		Block(shared_ptr<Context> context, vector<shared_ptr<Call>> calls)
				: context(std::move(context)), calls(move(calls)) {}

		shared_ptr<Object> execute(const vector<string> &params, const vector<shared_ptr<Object>> &args) {
			shared_ptr<Object> returnValue = Void;
			context->associate(params, args);
			for (auto &call: calls) {
				returnValue = call->invoke();
			}
			return returnValue;
		}
	};

	struct Fun : public Object {
		vector<string> params;
		shared_ptr<Block> block;

		static const shared_ptr<Fun> Empty;

		TYPER(Fun);

		Fun(vector<string> params, shared_ptr<Block> block) : params(std::move(params)), block(std::move(block)) {
			addMethod("->", [this](const shared_ptr<Object> &other) -> shared_ptr<Object> {
				vector<shared_ptr<Object>> args;
				if (other->getType() == "Empty");
				else if (other->getType() == "Tuple") {
					args = reinterpret_pointer_cast<Tuple>(other)->values;
				} else {
					args.push_back(other);
				}
				if (this->params.size() == args.size()) {
					return this->block->execute(this->params, args);
				} else return Null;
			});
		}
	};

	struct Class : public Object {
		shared_ptr<Class> super;
		shared_ptr<Fun> body;

		TYPER(Class);

		explicit Class(shared_ptr<Fun> body) : body(std::move(body)) {
			addMethod("extend", [this](const shared_ptr<Object> &other) -> shared_ptr<Object> {
				//todo
				return shared_from_this();
			});
		};
	};

	const static shared_ptr<Empty> Void;
	const static shared_ptr<Nothing> Null;
	shared_ptr<Block> main;

	explicit poolx(const json &jsonAST);

	void execute(const vector<string> &args);

	shared_ptr<poolx::Object> parseTerm(const json &ast, const shared_ptr<Context> &context);

	shared_ptr<Call> parseCall(const json &ast, const shared_ptr<Context> &context);

	vector<string> parseArgs(const json &ast);

	shared_ptr<Fun> parseFun(const json &ast, const shared_ptr<Context> &context);

	shared_ptr<poolx::Block> parseBlock(const json &ast, const shared_ptr<Context> &parent = nullptr);

	shared_ptr<Decimal> parseDecimal(const json &ast);

	shared_ptr<Integer> parseInteger(const json &ast);

	shared_ptr<Bool> parseBool(const json &ast);

	shared_ptr<String> parseString(const json &ast);

	shared_ptr<Tuple> parseTuple(const json &ast, const shared_ptr<Context> &context);

	shared_ptr<Array> parseArray(const json &ast, const shared_ptr<Context> &context);

	shared_ptr<Object> parseIdent(const json &ast, const shared_ptr<Context> &context);

	void initNatives();

public:
	static void execute(const string &file, const vector<string> &args);
};