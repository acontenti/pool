#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include "util/macro.hpp"

using namespace std;

namespace pool {
	class POOL_PUBLIC Object;

	class POOL_PUBLIC Variable {
		shared_ptr<Object> value;
		bool immutable = false;
	public:
		const string name;

		[[nodiscard]] bool isImmutable() const;

		void setImmutable(bool _immutable);

		[[nodiscard]] const shared_ptr<Object> &getValue() const;

		void setValue(const shared_ptr<Object> &val);

		Variable(string name, shared_ptr<Object> value, bool immutable);
	};

	class POOL_PUBLIC Context {
		unordered_map<string, shared_ptr<Variable>> heap;
		const shared_ptr<Context> parent;
	public:
		static shared_ptr<Context> global;

		explicit Context(shared_ptr<Context> parent);

		[[nodiscard]] shared_ptr<Variable> find(const string &name) const;

		[[nodiscard]] shared_ptr<Variable> findLocal(const string &name) const;

		shared_ptr<Variable> add(const string &name);

		shared_ptr<Variable> set(const string &name, const shared_ptr<Object> &value, bool immutable = false);

		void remove(const string &name);

		[[nodiscard]] string toString(const Location &location) const;

		[[nodiscard]] inline auto begin() const {
			return heap.begin();
		}

		[[nodiscard]] inline auto end() const {
			return heap.end();
		}

		[[nodiscard]] inline auto empty() const {
			return heap.empty();
		}

		static shared_ptr<Context> create(const shared_ptr<Context> &parent);
	};
}