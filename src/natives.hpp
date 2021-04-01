#pragma once

#include <unordered_map>
#include "poolstd.hpp"

using namespace std;

namespace pool {
	class Natives {
	public:
		virtual bool add(const string &name, const shared_ptr<Object> &value) = 0;

		virtual bool addFun(const string &name, const vector<Fun::Param> &parameters, const NativeFun::method_t &code) = 0;

		virtual shared_ptr<Object> find(const string &name) = 0;

		static Natives &get();
	};
}