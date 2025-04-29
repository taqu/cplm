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
	std::vector<int32_t> tokens = tokenizer.encode(text, 0);
	std::u8string decoded = tokenizer.decode(static_cast<int32_t>(tokens.size()), &tokens[0]);
	printf("%s\n", (const char*)decoded.c_str());
}

TEST_CASE("Test Model" "[cplm]")
{
	using namespace cplm;
	Model model;
	bool r = model.open("tinyllama.calm", 128);
	CHECK(r);
	const float* logits = model.forward(0, 0, 0);
	Model::Params params;
	params.temperature_ = 0.0f;
	Result result = model.generate_one(u8"Q: What is the meaning of life?", params);

	fprintf(stdout, "%s\n", (const char*)result.text_.c_str());
	fprintf(stderr, "# %d tokens: throughput: %.2f tok/s; latency: %.2f ms/tok; bandwidth: %.2f GB/s; total %.3f sec; #%08x\n",
	        result.num_tokens_,
	        result.num_tokens_ / result.duration_ * 1000, result.duration_ / result.num_tokens_,
	        ((double)result.read_bytes_ / 1.0e9) / (result.duration_ / 1000),
	        result.duration_ / 1000, result.logits_hash_);

}

