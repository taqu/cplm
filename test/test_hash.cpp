#include "catch_amalgamated.hpp"
#include <cstdint>
#include <vector>
#include <random>
#include <string>
#include <algorithm>
#include "cppgpt.h"

namespace
{
	static const char* ASCII = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-=+|;:'[{]}/?.>,<@#$%^&*()";
}

namespace cppgpt
{
	//--- Hasher
//-----------------------------------------------------------
template<>
struct Hasher<std::string>
{
    u32 operator()(const std::string& x) const
    {
		return cppgpt::wyhash32(x.length(), x.c_str());
    }
};

}

std::string random_string(std::mt19937& engine, uint32_t minx, uint32_t maxx)
{
	assert(minx<maxx);
	uint32_t s = maxx - minx;
	uint32_t l = (engine()%s) + minx;
	std::string str;
    str.reserve(l);
	uint32_t r = ::strlen(ASCII);
    for(uint32_t i=0; i<l; ++i){
		uint32_t index = engine()%r;
        str.push_back(ASCII[index]);
    }
	return str;
}

TEST_CASE("HashMap" "[CPPGPT]")
{
	static constexpr uint32_t Samples = 9999;
	static constexpr uint32_t Half = Samples/2;
	static constexpr uint32_t Min = 3;
	static constexpr uint32_t Max = 23;
	std::mt19937 engine;
    {
		std::random_device device;
		engine.seed(device());
    }
	std::vector<std::string> keys;
    {
		uint32_t count = 0;
		while(count<Samples){
			std::string str = random_string(engine, Min, Max);
			CHECK(Min<=str.length());
			CHECK(str.length()<=Max);
			auto itr = std::find(keys.begin(), keys.end(), str);
			if(itr != keys.end()){
				continue;
			}
			keys.push_back(str);
			++count;
		}
    }
	cppgpt::HashMap<std::string, std::string> hashMap(128);
    for(uint32_t i = 0; i < Samples; ++i) {
        hashMap.add(keys[i], keys[i]);
    }
	for(uint32_t i=0; i<Samples; ++i){
		uint32_t pos = hashMap.find(keys[i]);
		CHECK(pos != hashMap.end());
		CHECK(keys[i] == hashMap.getValue(pos));
	}
	for(uint32_t i=0; i<Half; ++i){
		hashMap.remove(keys[i]);
	}
	for(uint32_t i=0; i<Half; ++i){
		uint32_t pos = hashMap.find(keys[i]);
		CHECK(pos == hashMap.end());
	}
	for(uint32_t i=Half; i<Samples; ++i){
		uint32_t pos = hashMap.find(keys[i]);
		CHECK(pos != hashMap.end());
		CHECK(keys[i] == hashMap.getValue(pos));
	}
	for(uint32_t i=Half; i<Samples; ++i){
		hashMap.remove(keys[i]);
	}
	for(uint32_t i=0; i<Samples; ++i){
		uint32_t pos = hashMap.find(keys[i]);
		CHECK(pos == hashMap.end());
	}
}

