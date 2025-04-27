#include "catch_amalgamated.hpp"
#include <cstdint>
#include <random>
#include <string>
#include <iostream>
#include "cppgpt.h"

TEST_CASE("PriorityQueue" "[CPPGPT]")
{
	using namespace cppgpt;
	static constexpr uint32_t Samples = 100;
	static constexpr uint32_t Half = Samples/2;
	std::mt19937 engine;
    {
		std::random_device device;
		engine.seed(device());
    }
	PriorityQueue<uint32_t> values;
    {
		uint32_t count = 0;
		while(count<Samples){
			uint32_t value = engine();
			values.push_back(value);
			++count;
		}
		uint32_t last_value = 0xFFFF'FFFFUL;
		while(0<values.size()){
			uint32_t x = values.front();
			values.pop_front();
			std::cout << x << std::endl;
			CHECK(x<=last_value);
			last_value = x;
		}
    }
}

