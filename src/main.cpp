#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "cplm.h"

void error_usage()
{
    fprintf(stderr, "Usage:   run <checkpoint> [options]\n");
    fprintf(stderr, "Example: run model.bin -n 256 -i \"Once upon a time\"\n");
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "  -t <float>  temperature in [0,inf], default 1.0\n");
    fprintf(stderr, "  -p <float>  p value in min-p (cutoff) sampling in [0,1] default 0.1\n");
    fprintf(stderr, "  -s <int>    random seed, default time(NULL)\n");
    fprintf(stderr, "  -n <int>    number of steps to run for, default 256. 0 = max_seq_len, -1 = infinite\n");
    fprintf(stderr, "  -r <int>    number of sequences to decode, default 1\n");
    fprintf(stderr, "  -c <int>    context length, default to model-specific maximum\n");
    fprintf(stderr, "  -i <string> input prompt (- to read from stdin)\n");
    fprintf(stderr, "  -x <path>   compute perplexity for text file\n");
    fprintf(stderr, "  -y <string> chat mode with a system prompt\n");
}

int main(int argc, char* argv[])
{
    // default parameters
	char* checkpoint_path = nullptr;    // e.g. out/model.bin
	char* prompt = nullptr;             // prompt string
	char* perplexity = nullptr;         // text file for perplexity
	char* system_prompt = nullptr;      // chat system prompt
	cplm::Model::Params params;
    params.temperature_ = 1.0f; // 0.0 = greedy deterministic. 1.0 = original. don't set higher
	params.minp_ = 0.1f; // min-p sampling. 0.0 = off
	params.steps_ = 256; // number of steps to run for
	params.sequences_ = 1; // number of sequences to decode
	params.seed_ = 0; // seed rng with time by default
	params.context_ = 0; // context length
	int64_t seed = 0;

	// poor man's C argparse so we can override the defaults above from the command line
	if (argc >= 2) {
		checkpoint_path = argv[1];
	} else {
		error_usage();
		return 1;
	}
	for (int32_t i = 2; i < argc; i += 2) {
		// do some basic validation
		if (i + 1 >= argc) {
			error_usage();
			return 1;
		} // must have arg after flag
		if (argv[i][0] != '-') {
			error_usage();
			return 1;
		} // must start with dash
		if (strlen(argv[i]) != 2) {
			error_usage();
			return 1;
		} // must be -x (one dash, one letter)
		// read in the args
		if (argv[i][1] == 't') {
			params.temperature_ = atof(argv[i + 1]);
		} else if (argv[i][1] == 'p') {
			params.minp_ = atof(argv[i + 1]);
		} else if (argv[i][1] == 's') {
			seed = atoll(argv[i + 1]);
		} else if (argv[i][1] == 'n') {
			params.steps_ = atoi(argv[i + 1]);
		} else if (argv[i][1] == 'r') {
			params.sequences_ = atoi(argv[i + 1]);
		} else if (argv[i][1] == 'i') {
			prompt = argv[i + 1];
		} else if (argv[i][1] == 'x') {
			perplexity = argv[i + 1];
		} else if (argv[i][1] == 'c') {
			params.context_ = atoi(argv[i + 1]);
		} else if (argv[i][1] == 'y') {
			system_prompt = argv[i + 1];
		} else {
			error_usage();
			return 1;
		}
	}

	if(nullptr == checkpoint_path || nullptr == prompt) {
		return 1;
	}
	// parameter validation/overrides
	if (seed <= 0){
		params.seed_ = (uint64_t)time(NULL);
	}else{
		params.seed_ = (uint64_t)seed;
	}
    {
        using namespace cplm;
        Model model;
        bool r = model.open(checkpoint_path, params.context_);
		if(!r){
			return 1;
		}
        Result result = model.generate_one((const char8_t*)prompt, params);

        fprintf(stdout, "%s\n", (const char*)result.text_.c_str());
        fprintf(stderr, "# %d tokens: throughput: %.2f tok/s; latency: %.2f ms/tok; bandwidth: %.2f GB/s; total %.3f sec; #%08x\n",
                result.num_tokens_,
                result.num_tokens_ / result.duration_ * 1000, result.duration_ / result.num_tokens_,
                ((double)result.read_bytes_ / 1.0e9) / (result.duration_ / 1000),
                result.duration_ / 1000, result.logits_hash_);
    }
	return 0;
}