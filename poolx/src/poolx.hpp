#pragma once

#include "../../lib/json.hpp"
#include "poolstd.hpp"

using json = nlohmann::json;
using namespace std;

namespace pool {
	class PoolX {
		shared_ptr<Block> main;

		shared_ptr<Object> parseTerm(const json &ast, const shared_ptr<Context> &context);

		shared_ptr<Call> parseCall(const json &ast, const shared_ptr<Context> &context);

		shared_ptr<Call> parseInvocation(const json &ast, const shared_ptr<Context> &context);

		shared_ptr<Block> parseBlock(const json &ast, const shared_ptr<Context> &parent);

		static shared_ptr<Decimal> parseDecimal(const json &ast, const shared_ptr<Context> &context);

		static shared_ptr<Integer> parseInteger(const json &ast, const shared_ptr<Context> &context);

		static shared_ptr<Bool> parseBool(const json &ast, const shared_ptr<Context> &context);

		static shared_ptr<String> parseString(const json &ast, const shared_ptr<Context> &context);

		shared_ptr<Tuple> parseTuple(const json &ast, const shared_ptr<Context> &context);

		shared_ptr<Array> parseArray(const json &ast, const shared_ptr<Context> &context);

		static shared_ptr<Variable> parseIdent(const json &ast, const shared_ptr<Context> &context);

		explicit PoolX(const json &jsonAST);

	public:
		constexpr static const std::string_view VERSION = POOL_VERSION;

		static shared_ptr<PoolX> load(const string &file, bool debug);

		void execute(const vector<string> &args);
	};
}