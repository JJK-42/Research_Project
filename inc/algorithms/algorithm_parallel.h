#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <mutex>

#include "game/game.h"
#include "statistics/stats.h"

const float ALPHA = -1000000.0;
const float BETA = 1000000.0;

class ParallelAlgorithm
{
	public:
		virtual int run_algorithm(int argc, char** argv, Game game) = 0;
		
	protected:
		Statistics stats;
		
		ParallelAlgorithm();
		
		float get_alpha(int state);
		float get_beta(int state);
		
		void store_alpha(int state, float val);
		void store_beta(int state, float val);
		
		void clear_bounds();
};

#endif
