#include "catch_amalgamated.hpp"
#include <vector>
#include <random>
#include "gguf.h"
#include "cppgpt.h"

TEST_CASE("Load GGUF" "[GGUF]")
{
	using namespace gguf;
	GGUF gguf;
	gguf::Error result = gguf.load(u8"./data/tinyllama-1.1b-chat-v1.0.Q2_K.gguf");
	CHECK(gguf::Error::Success == result);
}

#if 0
TEST_CASE("Load Vocab" "[GGUF]")
{
	using namespace gguf;
	using namespace cppgpt;
	GGUF gguf;
	gguf::Error result = gguf.load(u8"./data/tinyllama-1.1b-chat-v1.0.Q2_K.gguf");
	CHECK(gguf::Error::Success == result);
	Tokens tokens(gguf);
	for(auto&& itr=tokens.getTokens().begin<GGUFString>(); itr; ++itr){
		GGUFString ggufStr = *itr;
		CHECK(ggufStr.length_<512);
		bool result;
		s32 token;
		result = tokens.encode(token, ggufStr.length_, ggufStr.str_);
		CHECK(result);
		char8_t decoded[512];
		result = tokens.decode(decoded, token);
		CHECK(result);
		u64 len = (std::min)(511ULL, ggufStr.length_);
		CHECK(0 == ::strncmp((const char*)ggufStr.str_, (const char*)decoded, len));
	}
}
#endif

