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
		Token *const start;
		Token *const end;

		Callable(Token *start, Token *end) : start(start), end(end) {}

		virtual shared_ptr<Object> invoke() = 0;
	};

	class Access : public Callable {
		shared_ptr<Callable> caller;
		string id;
	public:

		Access(Token *start, Token *end, shared_ptr<Callable> caller, string id)
				: Callable(start, end), caller(move(caller)), id(move(id)) {}

		shared_ptr<Object> invoke() override;

		static shared_ptr<Access> create(Token *start, Token *end, const shared_ptr<Callable> &caller, const string &id) {
			return make_shared<Access>(start, end, caller, id);
		}
	};

	class LocalAccess : public Callable {
		shared_ptr<Callable> caller;
		string id;
	public:

		LocalAccess(Token *start, Token *end, shared_ptr<Callable> caller, string id)
				: Callable(start, end), caller(move(caller)), id(move(id)) {}

		shared_ptr<Object> invoke() override;

		static shared_ptr<LocalAccess> create(Token *start, Token *end, const shared_ptr<Callable> &caller, const string &id) {
			return make_shared<LocalAccess>(start, end, caller, id);
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
		shared_ptr<Callable> caller;
		shared_ptr<Args> args;
	public:

		Invocation(Token *start, Token *end, const shared_ptr<Callable> &caller, const shared_ptr<Args> &args)
				: Callable(start, end), caller(caller), args(args) {}

		shared_ptr<Object> invoke() override;

		static shared_ptr<Invocation> create(Token *start, Token *end, const shared_ptr<Callable> &caller, const shared_ptr<Args> &args) {
			return make_shared<Invocation>(start, end, caller, args);
		}
	};

	class InvocationAccess : public Callable {
		shared_ptr<Callable> caller;
		string id;
		shared_ptr<Args> args;
		Token *const idToken;
	public:

		InvocationAccess(Token *start, Token *end, Token *idToken, const shared_ptr<Callable> &caller, string id, const shared_ptr<Args> &args)
				: Callable(start, end), idToken(idToken), caller(caller), id(move(id)), args(args) {}

		shared_ptr<Object> invoke() override;

		static shared_ptr<InvocationAccess> create(Token *start, Token *end, Token *idToken, const shared_ptr<Callable> &caller, const string &id, const shared_ptr<Args> &args) {
			return make_shared<InvocationAccess>(start, end, idToken, caller, id, args);
		}
	};

	class Identity : public Callable {
		shared_ptr<Object> object;
	public:
		explicit Identity(Token *start, Token *end, const shared_ptr<Object> &object)
				: Callable(start, end), object(object) {}

		shared_ptr<Object> invoke() override;

		static shared_ptr<Callable> create(Token *start, Token *end, const shared_ptr<Object> &caller) {
			return make_shared<Identity>(start, end, caller);
		}
	};

	class Assignment : public Callable {
		shared_ptr<Callable> assignee;
		shared_ptr<Callable> value;
		bool immutable;
	public:

		Assignment(Token *start, Token *end, shared_ptr<Callable> assignee, shared_ptr<Callable> value, bool immutable)
				: Callable(start, end), assignee(move(assignee)), value(move(value)), immutable(immutable) {}

		shared_ptr<Object> invoke() override;

		static shared_ptr<Assignment> create(Token *start, Token *end, const shared_ptr<Callable> &assignee, const shared_ptr<Callable> &value, bool immutable) {
			return make_shared<Assignment>(start, end, assignee, value, immutable);
		}
	};
}