#include <stdio.h>

#include "algorithms/algorithm.h"

using namespace std;

Algorithm::Algorithm()
{
	stats = new Statistics();
	hash_table = new Hashtable(stats);
}

Algorithm::~Algorithm()
{
	delete hash_table;
	delete stats;
}

void Algorithm::clear_bounds()
{
	delete hash_table;
	hash_table = new Hashtable(stats);
}

/*
int Algorithm::run_algorithm(int argc, char** argv, Game game)
{
	printf("run_algorithm(...) is not implemented.\n");
	return 0;
}*/
