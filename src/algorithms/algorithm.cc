#include <stdio.h>

#include "algorithms/algorithm.h"

using namespace std;

/**
 * Algorithm constructor. Creates a new Statistics object and a new
 * Hashtable object with stats as its constructor argument.
 */
Algorithm::Algorithm()
{
	stats = new Statistics();
	hash_table = new Hashtable(stats);
}

/**
 * Algorithm constructor. Creates a new Statistics object and sets
 * the hash_table to ht. Note that stats will not be refered to by
 * hash_table.
 */
Algorithm::Algorithm(Hashtable* htable)
{
	stats = new Statistics();
	hash_table = htable;
}

/**
 * Algorithm destructor.
 */
Algorithm::~Algorithm(){}

/**
 * Deletes hash_table and creates a new one. Does not create a new
 * stats object.
 */
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
