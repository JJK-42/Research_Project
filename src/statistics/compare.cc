
#include <cstdio>

#include "algorithm.h"
#include "ab_rollouts.h"
#include "ab_simple.h"

#define __MAIN__

void compare(int iters, vector<Algorithm> algorithms)
{
	for(int i = 0; i < iters; i++)
	{
		char seed[16]
		itoa(i, seed, 10);
		int argc = 3;
		char* argv[argc];
		argv[1] = "f";
		argv[2] = seed;
		
		for(Algorithm alg : algorithms)
			alg.run_algorithm(argc, argv);
	}
}

int main(int argc, char** argv)
{
	if(argc > 1)
	{
		printf("Comparing %i times.\n", argv[1]);
		
		vector<Algorithm> algorithms;
		
		Rollouts roll;
		Alphabeta ab;
		
		algorithms[0] = roll;
		algorithms[1] = ab;
		
		compare(argv[1], algorithms);
	}
	else
		printf("Usage: ./compare <#iterations>\n");
	return 0;
}
