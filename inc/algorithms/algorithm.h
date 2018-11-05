#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "game/game.h"
#include "statistics/stats.h"
#include "algorithms/hash_table.h"

class Algorithm
{
	public:
		virtual int run_algorithm(int argc, char** argv, Game game) = 0;
		
	protected:
		Statistics* stats;
		
		Algorithm();
		
		//float get_alpha(int state);
		//float get_beta(int state);
		
		//void store_alpha(int state, float val);
		//void store_beta(int state, float val);
		
		void clear_bounds();
		
		Hashtable* hash_table;//might need to be a pointer
};

#endif
