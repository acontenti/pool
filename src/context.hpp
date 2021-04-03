#pragma once

#include <unordered_map>
#include <string>
#include <memory>

using namespace std;

namespace pool {
	class Object;

	class Variable;

	class Context : public enable_shared_from_this<Context> {
		unordered_map<string, shared_ptr<Variable>> heap;
		const shared_ptr<Context> parent;
	public:
		static shared_ptr<Context> global;

		explicit Context(shared_ptr<Context> parent);

		shared_ptr<Variable> find(const string &name) const;

		shared_ptr<Variable> findLocal(const string &name) const;

		shared_ptr<Variable> add(const string &name);

		shared_ptr<Variable> set(const string &name, const shared_ptr<Object> &value, bool immutable = false);

		void remove(const string &name);

		string toString() const;

		inline auto begin() const {
			return heap.begin();
		}

		inline auto end() const {
			return heap.end();
		}

		inline auto empty() const {
			return heap.empty();
		}

		static shared_ptr<Context> create(const shared_ptr<Context> &parent);
	};
}