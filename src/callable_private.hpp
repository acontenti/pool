#pragma once

#include <callable.hpp>
#include <utility>
#include <vector>
#include <variant>
#include <string>

namespace pool {
	class Variable;

	class Context;

	class AssignmentAccess {
		shared_ptr<Callable> caller;
		bool local;
	public:
		string id;
		Location location;

		AssignmentAccess(Location location, shared_ptr<Callable> caller, string id, const bool &local)
				: location(move(location)), caller(move(caller)), id(move(id)), local(local) {}

		shared_ptr<Variable> invoke(const shared_ptr<Context> &context);

		static shared_ptr<AssignmentAccess> create(const Location &location, const shared_ptr<Callable> &caller, const string &id, const bool &local) {
			return make_shared<AssignmentAccess>(location, caller, id, local);
		}
	};

	class Access : public Callable {
	protected:
		shared_ptr<Callable> caller;
		string id;
		bool local;
	public:
		Access(const Location &location, shared_ptr<Callable> caller, string id, const bool &local)
				: Callable(location), caller(move(caller)), id(move(id)), local(local) {}

		shared_ptr<Object> invoke(const shared_ptr<Context> &context) override;

		static shared_ptr<Access> create(const Location &location, const shared_ptr<Callable> &caller, const string &id, const bool &local) {
			return make_shared<Access>(location, caller, id, local);
		}
	};

	class Expansion {
		shared_ptr<Callable> caller;
	public:
		explicit Expansion(const shared_ptr<Callable> &caller) : caller(caller) {}

		vector<shared_ptr<Object>> invoke(const shared_ptr<Context> &context);

		static shared_ptr<Expansion> create(const shared_ptr<Callable> &caller) {
			return make_shared<Expansion>(caller);
		}
	};

	class Args {
	public:
		using arg_t = variant<shared_ptr<Callable>, shared_ptr<Expansion>>;
	private:
		vector<arg_t> args;
	public:
		explicit Args(const vector<arg_t> &args) : args(args) {}

		vector<shared_ptr<Object>> invoke(const shared_ptr<Context> &context);

		static shared_ptr<Args> create(const vector<arg_t> &args) {
			return make_shared<Args>(args);
		}
	};

	class Invocation : public Callable {
	protected:
		shared_ptr<Callable> caller;
		shared_ptr<Args> args;
	public:
		Invocation(const Location &location, const shared_ptr<Callable> &caller, const shared_ptr<Args> &args)
				: Callable(location), caller(caller), args(args) {}

		shared_ptr<Object> invoke(const shared_ptr<Context> &context) override;

		static shared_ptr<Invocation> create(const Location &location, const shared_ptr<Callable> &caller, const shared_ptr<Args> &args) {
			return make_shared<Invocation>(location, caller, args);
		}
	};

	class InvocationAccess : public Callable {
	protected:
		shared_ptr<Callable> caller;
		shared_ptr<Args> args;
		string id;
		Location idLocation;
		bool local;
	public:
		InvocationAccess(const Location &location, Location idLocation, const shared_ptr<Callable> &caller, string id, const shared_ptr<Args> &args, const bool &local)
				: Callable(location), caller(caller), args(args), idLocation(move(idLocation)), id(move(id)),
				  local(local) {}

		shared_ptr<Object> invoke(const shared_ptr<Context> &context) override;

		static shared_ptr<InvocationAccess> create(const Location &location, const Location &idLocation, const shared_ptr<Callable> &caller, const string &id, const shared_ptr<Args> &args, const bool &local) {
			return make_shared<InvocationAccess>(location, idLocation, caller, id, args, local);
		}
	};

	class Identity : public Callable {
		shared_ptr<Object> object;
	public:
		explicit Identity(const Location &location, const shared_ptr<Object> &object)
				: Callable(location), object(object) {}

		shared_ptr<Object> invoke(const shared_ptr<Context> &context) override;

		static shared_ptr<Callable> create(const Location &location, const shared_ptr<Object> &caller) {
			return make_shared<Identity>(location, caller);
		}
	};

	class Assignment : public Callable {
		shared_ptr<AssignmentAccess> assignee;
		shared_ptr<Callable> value;
		bool immutable;
	public:

		Assignment(const Location &location, shared_ptr<AssignmentAccess> assignee, shared_ptr<Callable> value, bool immutable)
				: Callable(location), assignee(move(assignee)), value(move(value)), immutable(immutable) {}

		shared_ptr<Object> invoke(const shared_ptr<Context> &context) override;

		static shared_ptr<Assignment> create(const Location &location, const shared_ptr<AssignmentAccess> &assignee, const shared_ptr<Callable> &value, bool immutable) {
			return make_shared<Assignment>(location, assignee, value, immutable);
		}
	};

	class ParseBlock : public Callable {
		vector<shared_ptr<Callable>> statements;
	public:
		explicit ParseBlock(const Location &location, const vector<shared_ptr<Callable>> &statements)
				: Callable(location), statements(statements) {}

		shared_ptr<Object> invoke(const shared_ptr<Context> &context) override;

		static shared_ptr<ParseBlock> create(const Location &location, const vector<shared_ptr<Callable>> &statements) {
			return make_shared<ParseBlock>(location, statements);
		}
	};

	class ParseArray : public Callable {
		shared_ptr<Args> args;
	public:
		explicit ParseArray(const Location &location, const shared_ptr<Args> &args)
				: Callable(location), args(args) {}

		shared_ptr<Object> invoke(const shared_ptr<Context> &context) override;

		static shared_ptr<ParseArray> create(const Location &location, const shared_ptr<Args> &args) {
			return make_shared<ParseArray>(location, args);
		}
	};

	class ParseFunction : public Callable {
	public:
		struct ParamInfo {
			string name;
			Location nameLocation;
			string type;
			Location typeLocation;
			bool rest;
		};
	private:
		vector<ParamInfo> paramInfos;
		vector<shared_ptr<Callable>> statements;
	public:
		explicit ParseFunction(const Location &location, const vector<ParamInfo> &paramInfos, const vector<shared_ptr<Callable>> &statements)
				: Callable(location), paramInfos(paramInfos), statements(statements) {}

		shared_ptr<Object> invoke(const shared_ptr<Context> &context) override;

		static shared_ptr<ParseFunction> create(const Location &location, const vector<ParamInfo> &paramInfos, const vector<shared_ptr<Callable>> &statements) {
			return make_shared<ParseFunction>(location, paramInfos, statements);
		}
	};

	class ParseString : public Callable {
		string value;
	public:
		explicit ParseString(const Location &location, string value) : Callable(location), value(move(value)) {}

		shared_ptr<Object> invoke(const shared_ptr<Context> &context) override;

		static shared_ptr<ParseString> create(const Location &location, const string &value) {
			return make_shared<ParseString>(location, value);
		}
	};

	class ParseNative : public Callable {
		string id;
	public:
		explicit ParseNative(const Location &location, string id) : Callable(location), id(move(id)) {}

		shared_ptr<Object> invoke(const shared_ptr<Context> &context) override;

		static shared_ptr<ParseNative> create(const Location &location, const string &id) {
			return make_shared<ParseNative>(location, id);
		}
	};

	class ParseIdentifier : public Callable {
		string id;
	public:
		explicit ParseIdentifier(const Location &location, string id) : Callable(location), id(move(id)) {}

		shared_ptr<Object> invoke(const shared_ptr<Context> &context) override;

		static shared_ptr<ParseIdentifier> create(const Location &location, const string &id) {
			return make_shared<ParseIdentifier>(location, id);
		}
	};

	class ParseNumber : public Callable {
		string text;
		int base;
	public:
		explicit ParseNumber(const Location &location, string text, const int &base)
				: Callable(location), text(move(text)), base(base) {}

		shared_ptr<Object> invoke(const shared_ptr<Context> &context) override;

		static shared_ptr<ParseNumber> create(const Location &location, const string &text, const int &base) {
			return make_shared<ParseNumber>(location, text, base);
		}
	};
}