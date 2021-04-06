#pragma once

#include <utility>
#include <vector>
#include <variant>
#include <string>
#include <memory>
#include "util/location.hpp"

using namespace std;

namespace pool {
	class POOL_PUBLIC Object;

	class POOL_PUBLIC Callable {
	public:
		Location location;

		explicit Callable(const Location &location) : location(location) {}

		virtual shared_ptr<Object> invoke() = 0;
	};

	class POOL_PUBLIC Access : public Callable {
	protected:
		shared_ptr<Callable> caller;
		string id;
	public:

		Access(const Location &location, shared_ptr<Callable> caller, string id)
				: Callable(location), caller(move(caller)), id(move(id)) {}

		shared_ptr<Object> invoke() override;

		static shared_ptr<Access> create(const Location &location, const shared_ptr<Callable> &caller, const string &id) {
			return make_shared<Access>(location, caller, id);
		}
	};

	class POOL_PUBLIC LocalAccess : public Access {
	public:

		LocalAccess(const Location &location, const shared_ptr<Callable> &caller, const string &id)
				: Access(location, caller, id) {}

		shared_ptr<Object> invoke() override;

		static shared_ptr<LocalAccess> create(const Location &location, const shared_ptr<Callable> &caller, const string &id) {
			return make_shared<LocalAccess>(location, caller, id);
		}
	};

	class POOL_PUBLIC Expansion {
		shared_ptr<Callable> caller;
	public:
		explicit Expansion(const shared_ptr<Callable> &caller) : caller(caller) {}

		vector<shared_ptr<Object>> invoke();

		static shared_ptr<Expansion> create(const shared_ptr<Callable> &caller) {
			return make_shared<Expansion>(caller);
		}
	};

	class POOL_PUBLIC Args {
	public:
		using arg_t = variant<shared_ptr<Callable>, shared_ptr<Expansion>>;
	private:
		vector<arg_t> args;
	public:
		explicit Args(const vector<arg_t> &args) : args(args) {}

		vector<shared_ptr<Object>> invoke();

		static shared_ptr<Args> create(const vector<arg_t> &args) {
			return make_shared<Args>(args);
		}
	};

	class POOL_PUBLIC Invocation : public Callable {
	protected:
		shared_ptr<Callable> caller;
		shared_ptr<Args> args;
	public:

		Invocation(const Location &location, const shared_ptr<Callable> &caller, const shared_ptr<Args> &args)
				: Callable(location), caller(caller), args(args) {}

		shared_ptr<Object> invoke() override;

		static shared_ptr<Invocation> create(const Location &location, const shared_ptr<Callable> &caller, const shared_ptr<Args> &args) {
			return make_shared<Invocation>(location, caller, args);
		}
	};

	class POOL_PUBLIC InvocationAccess : public Callable {
	protected:
		shared_ptr<Callable> caller;
		shared_ptr<Args> args;
		string id;
		antlr4::Token *const idToken;
	public:

		InvocationAccess(const Location &location, antlr4::Token *idToken, const shared_ptr<Callable> &caller, string id, const shared_ptr<Args> &args)
				: Callable(location), caller(caller), args(args), idToken(idToken), id(move(id)) {}

		shared_ptr<Object> invoke() override;

		static shared_ptr<InvocationAccess> create(const Location &location, antlr4::Token *idToken, const shared_ptr<Callable> &caller, const string &id, const shared_ptr<Args> &args) {
			return make_shared<InvocationAccess>(location, idToken, caller, id, args);
		}
	};

	class POOL_PUBLIC InvocationLocalAccess : public InvocationAccess {
	public:
		InvocationLocalAccess(const Location &location, antlr4::Token *idToken, const shared_ptr<Callable> &caller, const string &id, const shared_ptr<Args> &args)
				: InvocationAccess(location, idToken, caller, id, args) {}

		shared_ptr<Object> invoke() override;

		static shared_ptr<InvocationLocalAccess> create(const Location &location, antlr4::Token *idToken, const shared_ptr<Callable> &caller, const string &id, const shared_ptr<Args> &args) {
			return make_shared<InvocationLocalAccess>(location, idToken, caller, id, args);
		}
	};

	class POOL_PUBLIC Identity : public Callable {
		shared_ptr<Object> object;
	public:
		explicit Identity(const Location &location, const shared_ptr<Object> &object)
				: Callable(location), object(object) {}

		shared_ptr<Object> invoke() override;

		static shared_ptr<Callable> create(const Location &location, const shared_ptr<Object> &caller) {
			return make_shared<Identity>(location, caller);
		}
	};

	class POOL_PUBLIC Assignment : public Callable {
		shared_ptr<Callable> assignee;
		shared_ptr<Callable> value;
		bool immutable;
	public:

		Assignment(const Location &location, shared_ptr<Callable> assignee, shared_ptr<Callable> value, bool immutable)
				: Callable(location), assignee(move(assignee)), value(move(value)), immutable(immutable) {}

		shared_ptr<Object> invoke() override;

		static shared_ptr<Assignment> create(const Location &location, const shared_ptr<Callable> &assignee, const shared_ptr<Callable> &value, bool immutable) {
			return make_shared<Assignment>(location, assignee, value, immutable);
		}
	};
}