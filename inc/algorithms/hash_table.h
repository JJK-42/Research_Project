#ifndef HASH_TABLE_H
#define HASH_TABLE_H

extern "C"
{
	//#include <util/MurmurHash3.h>
	#include "util/hashtable.h"
}

#include "game/game.h"
#include "statistics/stats.h"
#include <limits.h>

const int ALPHA = 1;
const int BETA = INT_MAX;

class Hashtable
{
	public:
		Hashtable();
		Hashtable(Statistics* statistics);
		
		int get_alpha(Game* key);
		int get_beta(Game* key);
		bool swap_alpha(Game* key, int val);
		bool swap_beta(Game* key, int val);
		
		/**
		 * Ideally, make use of an atomic compare-and-swap operation.
		 * But, depending on the implementation of the table, may need
		 * to lock, compare, replace and then unlock.
		 */
		bool increase_alpha(Game* key, int val, int comp_val);
		/**
		 * Ideally, make use of an atomic compare-and-swap operation.
		 * But, depending on the implementation of the table, may need
		 * to lock, compare, replace and then unlock.
		 */
		bool increase_beta(Game* key, int val, int comp_val);
		
		void clear();
		
	private:
		Statistics* stats;
		
		hashtable_t* alpha_map;
		hashtable_t* beta_map;
};

#endif
