#include "catch_amalgamated.hpp"
#include "cplm.h"

TEST_CASE("Load calm" "[cplm]")
{
	using namespace cplm;
	int32_t count = getCudaDeviceCount();
	(void)count;

	Model model;
	bool result = model.open("qwen2.5-coder-1.5b.calm", 128);
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
	bool r = model.open("qwen2.5-coder-1.5b.calm", 1024);
	CHECK(r);
	const float* logits = model.forward(0, 0, 0);
	(void)logits;
	Model::Params params;
	params.context_ = 1024;
	params.steps_ = 1024;
	params.temperature_ = 0.0f;
	Result result = model.generate_one(u8"Q: What is the meaning of life?", params);

	fprintf(stdout, "%s\n", (const char*)result.text_.c_str());
	fprintf(stderr, "# %d tokens: throughput: %.2f tok/s; latency: %.2f ms/tok; bandwidth: %.2f GB/s; total %.3f sec; #%08x\n",
	        result.num_tokens_,
	        result.num_tokens_ / result.duration_ * 1000, result.duration_ / result.num_tokens_,
	        ((double)result.read_bytes_ / 1.0e9) / (result.duration_ / 1000),
	        result.duration_ / 1000, result.logits_hash_);
#if 0
	const char8_t* prompt =
u8"<|fim_prefix|>import datetime\n"
u8"def calculate_age(birth_year):\n"
u8"    \"\"\"Calculates a person's age based on their birth year.\"\"\"\n"
u8"    current_year = datetime.date.today().year\n"
u8"    <|fim_suffix|>\n"
u8"    return age<|fim_middle|>";
#endif
	#if 0
const char8_t* prompt = 
u8"<|fim_prefix|>def quicksort(arr):\n"
u8"    if len(arr) <= 1:\n"
u8"        return arr\n"
u8"    pivot = arr[len(arr) // 2]\n"
u8"    <|fim_suffix|>\n"
u8"    middle = [x for x in arr if x == pivot]\n"
u8"    right = [x for x in arr if x > pivot]\n"
u8"    return quicksort(left) + middle + quicksort(right)<|fim_middle|>";
#endif
const char8_t* prompt =
u8"<|fim_prefix|><|fim_suffix|>// clang-format off
/*
# License
This software is distributed under two licenses, choose whichever you like.

## MIT License
Copyright (c) 2022 Takuro Sakai

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHE<|fim_middle|>";

	result = model.generate_one(prompt, params);

	fprintf(stdout, "%s\n", (const char*)result.text_.c_str());
	fprintf(stderr, "# %d tokens: throughput: %.2f tok/s; latency: %.2f ms/tok; bandwidth: %.2f GB/s; total %.3f sec; #%08x\n",
	        result.num_tokens_,
	        result.num_tokens_ / result.duration_ * 1000, result.duration_ / result.num_tokens_,
	        ((double)result.read_bytes_ / 1.0e9) / (result.duration_ / 1000),
	        result.duration_ / 1000, result.logits_hash_);

}

