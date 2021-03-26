#pragma once

#include <vector>
#include <string>
#include <memory>

using namespace std;

namespace pool {
	class Object;

	class Callable {
	public:
		virtual shared_ptr<Object> invoke() = 0;
	};

	class Access : public Callable {
		shared_ptr<Callable> caller;
		string id;
	public:

		Access(shared_ptr<Callable> caller, string id) : caller(move(caller)), id(move(id)) {}

		shared_ptr<Object> invoke() override;

		static shared_ptr<Access> create(const shared_ptr<Callable> &caller, const string &id) {
			return make_shared<Access>(caller, id);
		}
	};

	class LocalAccess : public Callable {
		shared_ptr<Callable> caller;
		string id;
	public:

		LocalAccess(shared_ptr<Callable> caller, string id) : caller(move(caller)), id(move(id)) {}

		shared_ptr<Object> invoke() override;

		static shared_ptr<LocalAccess> create(const shared_ptr<Callable> &caller, const string &id) {
			return make_shared<LocalAccess>(caller, id);
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
		shared_ptr<Callable> self;
		shared_ptr<Callable> caller;
		shared_ptr<Args> args;
	public:

		Invocation(const shared_ptr<Callable> &self, const shared_ptr<Callable> &caller, const shared_ptr<Args> &args)
				: self(self), caller(caller), args(args) {}

		shared_ptr<Object> invoke() override;

		static shared_ptr<Invocation> create(const shared_ptr<Callable> &self, const shared_ptr<Callable> &caller, const shared_ptr<Args> &args) {
			return make_shared<Invocation>(self, caller, args);
		}
	};

	class Identity : public Callable {
		shared_ptr<Object> object;
	public:
		explicit Identity(const shared_ptr<Object> &object) : object(object) {}

		shared_ptr<Object> invoke() override;

		static shared_ptr<Callable> create(const shared_ptr<Object> &caller) {
			return make_shared<Identity>(caller);
		}
	};

	class Assignment : public Callable {
		shared_ptr<Callable> assignee;
		shared_ptr<Callable> value;
		bool immutable;
	public:

		Assignment(shared_ptr<Callable> assignee, shared_ptr<Callable> value, bool immutable)
				: assignee(move(assignee)), value(move(value)), immutable(immutable) {}

		shared_ptr<Object> invoke() override;

		static shared_ptr<Assignment> create(const shared_ptr<Callable> &assignee, const shared_ptr<Callable> &value, bool immutable) {
			return make_shared<Assignment>(assignee, value, immutable);
		}
	};
}