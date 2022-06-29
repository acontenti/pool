#include <natives.hpp>
#include <iostream>

using namespace pool;

[[maybe_unused]] const auto load = []() {
	Natives::get().addFun("externalTest", {}, [](const shared_ptr<Object> &self, const vector<shared_ptr<Object>> &other, const Location &location) -> shared_ptr<Object> {
		std::cout << "Greetings from external-test!" << std::endl;
		return Null;
	});
	return true;
}();