#include <stdio.h>

#include "algorithms/algorithm.h"

using namespace std;

Algorithm::Algorithm()
{
	//hash_table = new Hashtable(stats);
	//alpha.resize(1, ALPHA);
	//beta.resize(1, BETA);
}

/**
 * If alpha[state] has been set, return alpha[state]
 * else, initialise alpha[state] to ALPHA (which should be a value
 * less than or equal to every possible game state evaluation).
 *//*
float Algorithm::get_alpha(int state)
{
	while((int)alpha.size() <= state)
		alpha.resize(alpha.size() * 2, ALPHA);
	return alpha[state];
}*/

/**
 * If beta[state] has been set, return beta[state]
 * else, initialise beta[state] to BETA (which should be a value
 * greater than or equal to every possible game state evaluation).
 *//*
float Algorithm::get_beta(int state)
{
	while((int)beta.size() <= state)
		beta.resize(beta.size() * 2, BETA);
	return beta[state];
}*/
/*
void Algorithm::store_alpha(int state, float val)
{
	alpha[state] = val;
}*/
/*
void Algorithm::store_beta(int state, float val)
{
	beta[state] = val;
}*/

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
