#pragma once

#include <utility>
#include <vector>
#include <string>
#include <memory>
#include "Token.h"

using namespace std;
using namespace antlr4;

namespace pool {
	class Object;

	class Callable {
	public:
		const pair<Token *, Token *> location;

		explicit Callable(const pair<Token *, Token *> &location) : location(location) {}

		virtual shared_ptr<Object> invoke() = 0;
	};

	class Access : public Callable {
	protected:
		shared_ptr<Callable> caller;
		string id;
	public:

		Access(const pair<Token *, Token *> &location, shared_ptr<Callable> caller, string id)
				: Callable(location), caller(move(caller)), id(move(id)) {}

		shared_ptr<Object> invoke() override;

		static shared_ptr<Access> create(const pair<Token *, Token *> &location, const shared_ptr<Callable> &caller, const string &id) {
			return make_shared<Access>(location, caller, id);
		}
	};

	class LocalAccess : public Access {
	public:

		LocalAccess(const pair<Token *, Token *> &location, const shared_ptr<Callable> &caller, const string &id)
				: Access(location, caller, id) {}

		shared_ptr<Object> invoke() override;

		static shared_ptr<LocalAccess> create(const pair<Token *, Token *> &location, const shared_ptr<Callable> &caller, const string &id) {
			return make_shared<LocalAccess>(location, caller, id);
		}
	};

	class Expansion {
		shared_ptr<Callable> caller;
	public:
		explicit Expansion(const shared_ptr<Callable> &caller) : caller(caller) {}

		vector<shared_ptr<Object>> invoke();

		static shared_ptr<Expansion> create(const shared_ptr<Callable> &caller) {
			return make_shared<Expansion>(caller);
		}
	};

	class Args {
		vector<shared_ptr<Callable>> args;
		shared_ptr<Expansion> rest;
	public:
		Args(const vector<shared_ptr<Callable>> &args, const shared_ptr<Expansion> &rest) : args(args), rest(rest) {}

		vector<shared_ptr<Object>> invoke();

		static shared_ptr<Args> create(const vector<shared_ptr<Callable>> &args, const shared_ptr<Expansion> &rest) {
			return make_shared<Args>(args, rest);
		}
	};

	class Invocation : public Callable {
	protected:
		shared_ptr<Callable> caller;
		shared_ptr<Args> args;
	public:

		Invocation(const pair<Token *, Token *> &location, const shared_ptr<Callable> &caller, const shared_ptr<Args> &args)
				: Callable(location), caller(caller), args(args) {}

		shared_ptr<Object> invoke() override;

		static shared_ptr<Invocation> create(const pair<Token *, Token *> &location, const shared_ptr<Callable> &caller, const shared_ptr<Args> &args) {
			return make_shared<Invocation>(location, caller, args);
		}
	};

	class InvocationAccess : public Invocation {
	protected:
		string id;
		Token *const idToken;
	public:

		InvocationAccess(const pair<Token *, Token *> &location, Token *idToken, const shared_ptr<Callable> &caller, string id, const shared_ptr<Args> &args)
				: Invocation(location, caller, args), idToken(idToken), id(move(id)) {}

		shared_ptr<Object> invoke() override;

		static shared_ptr<InvocationAccess> create(const pair<Token *, Token *> &location, Token *idToken, const shared_ptr<Callable> &caller, const string &id, const shared_ptr<Args> &args) {
			return make_shared<InvocationAccess>(location, idToken, caller, id, args);
		}
	};

	class InvocationLocalAccess : public InvocationAccess {
	public:
		InvocationLocalAccess(const pair<Token *, Token *> &location, Token *idToken, const shared_ptr<Callable> &caller, const string &id, const shared_ptr<Args> &args)
				: InvocationAccess(location, idToken, caller, id, args) {}

		shared_ptr<Object> invoke() override;

		static shared_ptr<InvocationLocalAccess> create(const pair<Token *, Token *> &location, Token *idToken, const shared_ptr<Callable> &caller, const string &id, const shared_ptr<Args> &args) {
			return make_shared<InvocationLocalAccess>(location, idToken, caller, id, args);
		}
	};

	class Identity : public Callable {
		shared_ptr<Object> object;
	public:
		explicit Identity(const pair<Token *, Token *> &location, const shared_ptr<Object> &object)
				: Callable(location), object(object) {}

		shared_ptr<Object> invoke() override;

		static shared_ptr<Callable> create(const pair<Token *, Token *> &location, const shared_ptr<Object> &caller) {
			return make_shared<Identity>(location, caller);
		}
	};

	class Assignment : public Callable {
		shared_ptr<Callable> assignee;
		shared_ptr<Callable> value;
		bool immutable;
	public:

		Assignment(const pair<Token *, Token *> &location, shared_ptr<Callable> assignee, shared_ptr<Callable> value, bool immutable)
				: Callable(location), assignee(move(assignee)), value(move(value)), immutable(immutable) {}

		shared_ptr<Object> invoke() override;

		static shared_ptr<Assignment> create(const pair<Token *, Token *> &location, const shared_ptr<Callable> &assignee, const shared_ptr<Callable> &value, bool immutable) {
			return make_shared<Assignment>(location, assignee, value, immutable);
		}
	};
}