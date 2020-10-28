#pragma once

#include <sstream>
#include <utility>
#include <vector>
#include "../../lib/json.hpp"
#include "util.hpp"

using json = nlohmann::json;
using namespace std;

#define TYPER(TYPE) string_view getType() const override{\
    return #TYPE;\
}

namespace pool {
	class poolx {
		struct Context;

		struct Object : enable_shared_from_this<Object> {
			typedef function<shared_ptr<Object>(const shared_ptr<Object> &)> method_t;
		private:
			static long ID_COUNTER;
			unordered_map<string, method_t> methodsMap;
			const long id = ID_COUNTER++;
		public:
			shared_ptr<Context> context;

			explicit Object(shared_ptr<Context> context) : context(move(context)) {
				addMethod("toString", [this](const shared_ptr<Object> &other) {
					return make_shared<String>(toString(other), this->context);
				});
			}

			virtual string_view getType() const {
				return "Object";
			};

			string toString(const shared_ptr<Object> &other) const {
				return string(getType()) + "@" + to_string(getID());
			};

			virtual const method_t *findMethod(const string &name) const {
				auto iterator = methodsMap.find(name);
				return iterator != methodsMap.end() ? &iterator->second : nullptr;
			};

			void addMethod(const string &name, const method_t &method) {
				methodsMap[name] = method;
			};

			long getID() const {
				return id;
			}
		};

		struct Callable {
			virtual shared_ptr<Object> invoke() = 0;
		};

		struct Empty : public Object {
			TYPER(Empty);

			Empty() : Object(Context::global) {
				addMethod("toString", [this](const shared_ptr<Object> &other) -> shared_ptr<Object> {
					static auto toString = make_shared<String>("void", this->context);
					return toString;
				});
			}
		};

		struct Nothing : public Object {
			TYPER(Nothing);

			Nothing() : Object(Context::global) {
				addMethod("toString", [this](const shared_ptr<Object> &other) -> shared_ptr<Object> {
					static auto toString = make_shared<String>("null", this->context);
					return toString;
				});
			}
		};

		struct Bool : public Object {
			bool value;

			TYPER(Bool);

			explicit Bool(bool value, const shared_ptr<Context> &context) : Object(context), value(value) {
				addMethod("toString", [this](const shared_ptr<Object> &other) -> shared_ptr<Object> {
					return make_shared<String>(this->value ? "true" : "false", this->context);
				});
			}
		};

		struct Integer : public Object {
			int64_t value;

			TYPER(Integer);

			explicit Integer(int64_t value, const shared_ptr<Context> &context) : Object(context), value(value) {
				addMethod("+", [this](const shared_ptr<Object> &other) -> shared_ptr<Object> {
					shared_ptr<Object> var = other;
					if (other->getType() == "Variable") {
						var = reinterpret_pointer_cast<Variable>(other)->value;
					}
					if (var->getType() == "Integer") {
						return make_shared<Integer>(
								this->value + reinterpret_pointer_cast<Integer>(var)->value, this->context);
					} else if (var->getType() == "Decimal") {
						return make_shared<Decimal>(
								this->value + reinterpret_pointer_cast<Decimal>(var)->value, this->context);
					} else if (var->getType() == "String") {
						return make_shared<String>(
								to_string(this->value) + reinterpret_pointer_cast<String>(var)->value, this->context);
					} else return Null;
				});
				addMethod("toString", [this](const shared_ptr<Object> &other) -> shared_ptr<Object> {
					return make_shared<String>(to_string(this->value), this->context);
				});
			}
		};

		struct Decimal : public Object {
			double value;

			TYPER(Decimal);

			explicit Decimal(double value, const shared_ptr<Context> &context) : Object(context), value(value) {
				addMethod("+", [this](const shared_ptr<Object> &other) -> shared_ptr<Object> {
					shared_ptr<Object> var = other;
					if (other->getType() == "Variable") {
						var = reinterpret_pointer_cast<Variable>(other)->value;
					}
					if (var->getType() == "Integer") {
						return make_shared<Decimal>(
								this->value + reinterpret_pointer_cast<Integer>(var)->value, this->context);
					} else if (var->getType() == "Decimal") {
						return make_shared<Decimal>(
								this->value + reinterpret_pointer_cast<Decimal>(var)->value, this->context);
					} else if (var->getType() == "String") {
						return make_shared<String>(
								to_string(this->value) + reinterpret_pointer_cast<String>(var)->value, this->context);
					} else return Null;
				});
				addMethod("toString", [this](const shared_ptr<Object> &other) -> shared_ptr<Object> {
					return make_shared<String>(to_string(this->value), this->context);
				});
			}
		};

		struct String : public Object {
			string value;

			TYPER(String);

			explicit String(string value, const shared_ptr<Context> &context) : Object(context), value(move(value)) {
				addMethod("+", [this](const shared_ptr<Object> &other) -> shared_ptr<Object> {
					shared_ptr<Object> var = other;
					if (other->getType() == "Variable") {
						var = reinterpret_pointer_cast<Variable>(other)->value;
					}
					if (auto method = var->findMethod("toString")) {
						auto result = (*method)(Tuple::Empty);
						if (result->getType() == "String") {
							return make_shared<String>(
									this->value + reinterpret_pointer_cast<String>(result)->value, this->context);
						}
					}
					return Null;
				});
				addMethod("toString", [this](const shared_ptr<Object> &other) -> shared_ptr<Object> {
					return shared_from_this();
				});
			}
		};

		struct Tuple : public Object {
			vector<shared_ptr<Object>> values;
			static const shared_ptr<Tuple> Empty;

			TYPER(Tuple);

			explicit Tuple(vector<shared_ptr<Object>> values, const shared_ptr<Context> &context)
					: Object(context), values(move(values)) {}
		};

		struct Array : public Object {
			vector<shared_ptr<Object>> values;

			TYPER(Array);

			explicit Array(vector<shared_ptr<Object>> values, const shared_ptr<Context> &context)
					: Object(context), values(move(values)) {}
		};

		struct Call : public Object, public Callable {
			shared_ptr<Object> caller;
			string method;
			shared_ptr<Object> callee;

			static const shared_ptr<Call> Empty;

			struct Identity : public Object {
				shared_ptr<Object> result;

				TYPER(Identity)

				explicit Identity(shared_ptr<Object> result, const shared_ptr<Context> &context)
						: Object(context), result(move(result)) {
					addMethod("", [this](const shared_ptr<Object> &other) -> shared_ptr<Object> {
						return this->result;
					});
				}
			};

			TYPER(Call);

			Call(shared_ptr<Object> caller, string method, shared_ptr<Object> callee, const shared_ptr<Context> &context)
					: Object(context), caller(move(caller)), method(move(method)), callee(move(callee)) {}

			shared_ptr<Object> invoke() override {
				auto first = caller, second = callee;
				if (auto callable = dynamic_pointer_cast<Callable>(caller)) {
					first = callable->invoke();
				}
				if (auto callable = dynamic_pointer_cast<Callable>(callee)) {
					second = callable->invoke();
				}
				auto function = first->findMethod(method);
				if (function) {
					return (*function)(second);
				} else throw execution_error(__FILE__, __LINE__, "method " + method + " not found");
			}

			static shared_ptr<Call> Identity(const shared_ptr<Object> &result, const shared_ptr<Context> &context) {
				return make_shared<Call>(make_shared<struct Identity>(result, context), "", Void, context);
			}
		};

		struct Variable : public Object {
			string name;
			shared_ptr<Object> value;

			TYPER(Variable);

			const method_t *findMethod(const string &methodName) const override {
				return Object::findMethod(methodName) ?: value->findMethod(methodName);
			}

			Variable(string name, shared_ptr<Object> value, const shared_ptr<Context> &context)
					: Object(context), name(move(name)), value(move(value)) {
				addMethod("=", [this](const shared_ptr<Object> &other) -> shared_ptr<Object> {
					shared_ptr<Object> var = other;
					if (other->getType() == "Variable") {
						var = reinterpret_pointer_cast<Variable>(other)->value;
					}
					this->value = var;
					return shared_from_this();
				});
				addMethod(".", [this](const shared_ptr<Object> &other) -> shared_ptr<Object> {
					if (other->getType() == "Variable") {
						auto id = reinterpret_pointer_cast<Variable>(other)->name;
						if (this->value->getType() == "Block") {
							const shared_ptr<Block> &block = reinterpret_pointer_cast<Block>(this->value);
							const shared_ptr<Object> &result = block->context->find(id);
							return result ?: Null;
						} else return Null;
					} else throw execution_error(__FILE__, __LINE__, "Invalid value for . call");
				});
			};

			explicit Variable(const string &name, const shared_ptr<Context> &context) : Variable(name, Null, context) {}
		};

		struct Context : enable_shared_from_this<Context> {
		private:
			unordered_map<string, shared_ptr<Variable>> heap;
			const shared_ptr<Context> parent;

			Context() : Context(nullptr) {}

		public:
			static const shared_ptr<Context> global;

			explicit Context(shared_ptr<Context> parent) : parent(move(parent)) {}

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
						add(name, make_shared<Variable>(name, value, shared_from_this()));
					}
				}
			}

			auto begin() {
				return heap.begin();
			}

			auto cbegin() const {
				return heap.cbegin();
			}

			auto end() {
				return heap.end();
			}

			auto cend() const {
				return heap.cend();
			}
		};

		struct Block : public Object, public Callable {
			vector<string> params;
			vector<shared_ptr<Call>> calls;
			static const shared_ptr<Block> Empty;

			TYPER(Block);

			Block(vector<string> params, vector<shared_ptr<Call>> calls, const shared_ptr<Context> &context)
					: Object(context), params(move(params)), calls(move(calls)) {
				for (auto &param : params) {
					context->add(param, make_shared<Variable>(param, context));
				}
				if (this->params.empty() && this->context != Context::global) {
					execute({});
				}
				addMethod("toString", [this](const shared_ptr<Object> &other) -> shared_ptr<Object> {
					stringstream ss;
					ss << "{";
					for (auto &item : *this->context) {
						shared_ptr<Object> var = item.second->value;
						if (var->getType() == "Variable") {
							var = reinterpret_pointer_cast<Variable>(other)->value;
						}
						if (auto method = var->findMethod("toString")) {
							auto result = (*method)(Tuple::Empty);
							ss << item.first << ":";
							if (result->getType() == "String") {
								ss << reinterpret_pointer_cast<String>(result)->value;
							} else {
								ss << result;
							}
							ss << ",";
						}
					}
					ss.seekp(-1, stringstream::cur); //remove last comma
					ss << "}";
					return make_shared<String>(ss.str(), this->context);
				});
				addMethod("->", [this](const shared_ptr<Object> &other) -> shared_ptr<Object> {
					vector<shared_ptr<Object>> args;
					if (other->getType() == "Empty");
					else if (other->getType() == "Tuple") {
						args = reinterpret_pointer_cast<Tuple>(other)->values;
					} else {
						args.push_back(other);
					}
					if (this->params.size() == args.size()) {
						return this->execute(args);
					} else return Null;
				});
			}

			shared_ptr<Object> invoke() override {
				return shared_from_this();
			}

			shared_ptr<Object> execute(const vector<shared_ptr<Object>> &args) {
				shared_ptr<Object> returnValue = Void;
				context->associate(params, args);
				for (auto &call: calls) {
					returnValue = call->invoke();
				}
				return returnValue;
			}
		};

		struct Class : public Object {
			shared_ptr<Class> super;
			shared_ptr<Block> body;

			TYPER(Class);

			explicit Class(shared_ptr<Block> body, const shared_ptr<Context> &context)
					: Object(context), body(move(body)) {
				addMethod("extend", [this](const shared_ptr<Object> &other) -> shared_ptr<Object> {
					//todo
					return shared_from_this();
				});
			};
		};

		const static shared_ptr<Empty> Void;
		const static shared_ptr<Nothing> Null;
		shared_ptr<Block> main;

		shared_ptr<poolx::Object> parseTerm(const json &ast, const shared_ptr<Context> &context);

		shared_ptr<Call> parseCall(const json &ast, const shared_ptr<Context> &context);

		shared_ptr<poolx::Block> parseBlock(const json &ast, const shared_ptr<Context> &parent = nullptr);

		static shared_ptr<Decimal> parseDecimal(const json &ast, const shared_ptr<Context> &context);

		static shared_ptr<Integer> parseInteger(const json &ast, const shared_ptr<Context> &context);

		static shared_ptr<Bool> parseBool(const json &ast, const shared_ptr<Context> &context);

		static shared_ptr<String> parseString(const json &ast, const shared_ptr<Context> &context);

		shared_ptr<Tuple> parseTuple(const json &ast, const shared_ptr<Context> &context);

		shared_ptr<Array> parseArray(const json &ast, const shared_ptr<Context> &context);

		static shared_ptr<Variable> parseIdent(const json &ast, const shared_ptr<Context> &context);

		shared_ptr<Call> createIdentSearchCall(const json &ast, json::const_reverse_iterator iter, const shared_ptr<Call> &other, const shared_ptr<Context> &context);

		static shared_ptr<Object> parseVariable(const json &ast, const shared_ptr<Context> &context);

		void initNatives();

	protected:
		explicit poolx(const json &jsonAST);

	public:
		static shared_ptr<poolx> load(const string &file, bool debug);

		void execute(const vector<string> &args);
	};

}