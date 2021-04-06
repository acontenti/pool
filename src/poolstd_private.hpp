#pragma once

#include "poolstd.hpp"
#include "parser.hpp"

namespace pool {
	class CodeFunction : public Function {
		vector<PoolParser::StatementContext *> calls;
	public:
		constexpr static const auto CREATOR = [](const shared_ptr<Context> &context, const any &_params, const any &_calls, const any &) {
			return make_shared<CodeFunction>(context, any_cast<vector<Param>>(_params), any_cast<vector<PoolParser::StatementContext *>>(_calls));
		};

		CodeFunction(const shared_ptr<Context> &context, const vector<Param> &params, vector<PoolParser::StatementContext *> calls);

		shared_ptr<Object> execute(const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) override;

		static shared_ptr<Object> newInstance(const shared_ptr<Context> &context, const Location &location, const vector<Param> &params, const vector<PoolParser::StatementContext *> &calls) {
			return FunctionClass->newInstance(context, location, {}, params, calls);
		}
	};

	class Block : public Executable {
	public:
		vector<PoolParser::StatementContext *> calls;
		constexpr static const auto CREATOR = [](const shared_ptr<Context> &context, const any &_calls, const any &, const any &) {
			return make_shared<Block>(context, any_cast<vector<PoolParser::StatementContext *>>(_calls));
		};

		Block(const shared_ptr<Context> &context, vector<PoolParser::StatementContext *> calls);

		shared_ptr<Object> execute(const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) override;

		shared_ptr<Object> execute(const Location &location);

		static shared_ptr<Object> newInstance(const shared_ptr<Context> &context, const Location &location, const vector<PoolParser::StatementContext *> &calls) {
			return BlockClass->newInstance(context, location, {}, calls);
		}
	};
}