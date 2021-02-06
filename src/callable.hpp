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

	class Invocation : public Callable {
		shared_ptr<Callable> self;
		shared_ptr<Callable> caller;
		vector<shared_ptr<Callable>> callee;
	public:

		Invocation(const shared_ptr<Callable> &self, shared_ptr<Callable> caller, const vector<shared_ptr<Callable>> &callee)
				: self(self), caller(move(caller)), callee(callee) {}

		shared_ptr<Object> invoke() override;

		static shared_ptr<Invocation> create(const shared_ptr<Callable> &self, const shared_ptr<Callable> &caller, const vector<shared_ptr<Callable>> &callee) {
			return make_shared<Invocation>(self, caller, callee);
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
}