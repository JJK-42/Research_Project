#ifndef AB_SIMPLE_H
#define AB_SIMPLE_H

#include "algorithms/algorithm.h"

class Alphabeta : public Algorithm
{
	public:
		Alphabeta();
		
		int run_algorithm(int argc, char** argv, Game game);
		
	protected:
		int expand(Game* game, int a, int b, int player, int depth, int& best);
		
		int alphabeta(Game* game, int a, int b, int player, int depth, int& best);
		int mt_sss(Game* game, int range, int player, int depth, int& best_move);
};

#endif
