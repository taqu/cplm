#include "catch_amalgamated.hpp"
#include "cplm.h"

TEST_CASE("Load calm" "[cplm]")
{
	using namespace cplm;
	Model model;
	bool result = model.open("tinyllm.calm", 128);
	CHECK(result);

	const Tensors& tensors = model.get_tensors();
	for(size_t i=0; i<tensors.num_metadata(); ++i){
		printf("%s %s\n", tensors.get_metadata(i).key_, tensors.get_metadata(i).value_);
	}
	const Tokenizer& tokenizer = model.get_tokenizer();
	const char8_t text[] = u8"ﾜｶﾞﾊｲは㈱である.吾輩は猫である。名前はまだない。<s></s>";
	std::vector<int32_t> tokens;
	tokens.resize(tokenizer.bound(sizeof(text)));
	int32_t num_tokens = tokenizer.encode(text, 0, &tokens[0]);
	std::u8string decoded = tokenizer.decode(num_tokens, &tokens[0]);
	printf("%s\n", (const char*)decoded.c_str());
}

TEST_CASE("Test Model" "[cplm]")
{
	using namespace cplm;
	Model model;
	bool r = model.open("tinyllama.calm", 256);
	CHECK(r);
	const float* logits = model.forward(0, 0, 0);
	Model::Params params;
	params.temperature_ = 0.0f;
	Result result = model.generate_one(u8"Q: What is the meaning of life?", params);
}

