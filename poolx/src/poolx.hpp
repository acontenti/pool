#pragma once

#include <sstream>
#include <utility>
#include <vector>
#include "../../lib/json.hpp"
#include "util.hpp"

using json = nlohmann::json;
using namespace std;

namespace pool {
	class poolx {
		struct Context;
		struct Class;

		struct Object : enable_shared_from_this<Object> {
		private:
			static long ID_COUNTER;
			const long id = ID_COUNTER++;
		public:
			static const shared_ptr<Class> ObjectClass;
			const shared_ptr<Class> cls;
			const shared_ptr<Context> context;

			Object(shared_ptr<Context> context, shared_ptr<Class> cls) : context(move(context)), cls(move(cls)) {
			}

			inline shared_ptr<Class> getClass() const {
				return cls;
			}

			virtual ~Object() = default;

			inline string_view getType() const {
				return cls->name;
			};

			string toString() const {
				return string(getType()) + "@" + to_string(getID());
			};

			inline long getID() const {
				return id;
			}

			template<class T>
			inline shared_ptr<T> as() {
				return reinterpret_pointer_cast<T>(shared_from_this());
			}
		};

		struct Class : public Object {
			typedef function<shared_ptr<Object>(const shared_ptr<Object> &, const shared_ptr<Object> &)> method_t;
			string name;
			shared_ptr<Class> super;
			unordered_map<string, method_t> methodsMap;
			static const shared_ptr<Class> ClassClass;

			Class(string name, shared_ptr<Class> super, shared_ptr<Context> context = Context::global)
					: Object(move(context), ClassClass), name(move(name)), super(move(super)) {
			}

			virtual const method_t *findMethod(const string &methodName) const {
				auto iterator = methodsMap.find(methodName);
				if (iterator != methodsMap.end()) {
					return &iterator->second;
				} else if (super) {
					return super->findMethod(methodName);
				} else return nullptr;
			};

			void addMethod(const string &methodName, const method_t &method) {
				methodsMap[methodName] = method;
			};
		};

		struct Callable : Object {
			Callable(shared_ptr<Context> context, shared_ptr<Class> cls) : Object(move(context), move(cls)) {};

			virtual shared_ptr<Object> invoke() = 0;
		};

		struct Empty : public Object {
			static const shared_ptr<Class> EmptyClass;

			Empty() : Object(Context::global, EmptyClass) {
			}
		};

		struct Nothing : public Object {
			static const shared_ptr<Class> NothingClass;

			Nothing() : Object(Context::global, NothingClass) {
			}
		};

		struct Bool : public Object {
			static const shared_ptr<Class> BoolClass;
			bool value;

			explicit Bool(bool value, const shared_ptr<Context> &context) : Object(context, BoolClass), value(value) {
			}
		};

		struct Integer : public Object {
			static const shared_ptr<Class> IntegerClass;
			int64_t value;

			explicit Integer(int64_t value, const shared_ptr<Context> &context)
					: Object(context, IntegerClass), value(value) {
			}
		};

		struct Decimal : public Object {
			static const shared_ptr<Class> DecimalClass;
			double value;

			explicit Decimal(double value, const shared_ptr<Context> &context)
					: Object(context, DecimalClass), value(value) {
			}
		};

		struct String : public Object {
			static const shared_ptr<Class> StringClass;
			string value;

			explicit String(string value, const shared_ptr<Context> &context)
					: Object(context, StringClass), value(move(value)) {
			}
		};

		struct Tuple : public Object {
			static const shared_ptr<Class> TupleClass;
			vector<shared_ptr<Object>> values;
			static const shared_ptr<Tuple> Empty;

			explicit Tuple(vector<shared_ptr<Object>> values, const shared_ptr<Context> &context)
					: Object(context, TupleClass), values(move(values)) {}
		};

		struct Array : public Object {
			static const shared_ptr<Class> ArrayClass;
			vector<shared_ptr<Object>> values;

			explicit Array(vector<shared_ptr<Object>> values, const shared_ptr<Context> &context)
					: Object(context, ArrayClass), values(move(values)) {}
		};

		struct Call : public Callable {
			static const shared_ptr<Class> CallClass;
			shared_ptr<Object> caller;
			string method;
			shared_ptr<Object> callee;

			static const shared_ptr<Call> Empty;

			struct Identity : public Object {
				static const shared_ptr<Class> IdentityClass;
				shared_ptr<Object> result;

				Identity(shared_ptr<Object> result, const shared_ptr<Context> &context)
						: Object(context, IdentityClass), result(move(result)) {
				}
			};

			Call(shared_ptr<Object> caller, string method, shared_ptr<Object> callee, const shared_ptr<Context> &context)
					: Callable(context, CallClass), caller(move(caller)), method(move(method)), callee(move(callee)) {}

			shared_ptr<Object> invoke() override {
				auto first = caller, second = callee;
				if (auto callable = dynamic_pointer_cast<Callable>(caller)) {
					first = callable->invoke();
				}
				if (auto callable = dynamic_pointer_cast<Callable>(callee)) {
					second = callable->invoke();
				}
				auto function = first->cls->findMethod(method);
				if (function) {
					return (*function)(first, second);
				} else throw execution_error(__FILE__, __LINE__, "method " + method + " not found");
			}

			static shared_ptr<Call> Identity(const shared_ptr<Object> &result, const shared_ptr<Context> &context) {
				return make_shared<Call>(make_shared<struct Identity>(result, context), "", Void, context);
			}
		};

		struct Variable : public Object {
			string name;
			shared_ptr<Object> value;

			struct VariableClass : public Class {
				shared_ptr<Object> value;

				VariableClass(shared_ptr<Object> value, const shared_ptr<Context> &context)
						: Class("Variable", Object::ObjectClass, context), value(move(value)) {
					addMethod("=", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
						shared_ptr<Object> var = other;
						if (other->getType() == "Variable") {
							var = other->as<Variable>()->value;
						}
						self->as<Variable>()->value = var;
						self->as<Variable>()->cls->as<VariableClass>()->value = var;
						return self->shared_from_this();
					});
					addMethod(".", [](const shared_ptr<Object> &self, const shared_ptr<Object> &other) -> shared_ptr<Object> {
						if (other->getType() == "Variable") {
							auto id = other->as<Variable>()->name;
							auto &value = self->as<Variable>()->value;
							if (value->getType() == "Block") {
								const shared_ptr<Object> &result = value->as<Block>()->context->find(id);
								return result ? result : Null;
							} else return Null;
						} else throw execution_error(__FILE__, __LINE__, "Invalid value for . call");
					});
				}

				const method_t *findMethod(const string &methodName) const override {
					const method_t *method = Class::findMethod(methodName);
					if (method) {
						return method;
					} else {
						return value->cls->findMethod(methodName);
					}
				}
			};

			Variable(string name, shared_ptr<Object> value, const shared_ptr<Context> &context)
					: Object(context, make_shared<VariableClass>(value, context)), name(move(name)),
					  value(move(value)) {
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

		struct Block : public Callable {
			static const shared_ptr<Class> BlockClass;
			vector<string> params;
			vector<shared_ptr<Call>> calls;
			static const shared_ptr<Block> Empty;

			Block(vector<string> params, vector<shared_ptr<Call>> calls, const shared_ptr<Context> &context)
					: Callable(context, BlockClass), params(move(params)), calls(move(calls)) {
				for (auto &param : params) {
					context->add(param, make_shared<Variable>(param, context));
				}
				if (this->params.empty() && this->context != Context::global) {
					execute({});
				}
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

		const static shared_ptr<Empty> Void;
		const static shared_ptr<Nothing> Null;
		shared_ptr<Block> main;

		shared_ptr<Object> parseTerm(const json &ast, const shared_ptr<Context> &context);

		shared_ptr<Call> parseCall(const json &ast, const shared_ptr<Context> &context);

		shared_ptr<Block> parseBlock(const json &ast, const shared_ptr<Context> &parent = nullptr);

		static shared_ptr<Decimal> parseDecimal(const json &ast, const shared_ptr<Context> &context);

		static shared_ptr<Integer> parseInteger(const json &ast, const shared_ptr<Context> &context);

		static shared_ptr<Bool> parseBool(const json &ast, const shared_ptr<Context> &context);

		static shared_ptr<String> parseString(const json &ast, const shared_ptr<Context> &context);

		shared_ptr<Tuple> parseTuple(const json &ast, const shared_ptr<Context> &context);

		shared_ptr<Array> parseArray(const json &ast, const shared_ptr<Context> &context);

		static shared_ptr<Variable> parseIdent(const json &ast, const shared_ptr<Context> &context);

		static void initNatives();

	protected:
		explicit poolx(const json &jsonAST);

	public:
		constexpr static const std::string_view VERSION = POOL_VERSION;

		static shared_ptr<poolx> load(const string &file, bool debug);

		void execute(const vector<string> &args);
	};
}