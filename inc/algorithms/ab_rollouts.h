#ifndef AB_ROLLOUTS_H
#define AB_ROLLOUTS_H

#include <vector>

#include "algorithms/algorithm.h"

class Rollouts : public Algorithm
{
	public:
		Rollouts();
		~Rollouts();
		
		int run_algorithm(int argc, char** argv, Game* game);
		
	protected:
		int evaluate(Game* game);
		Game* select(std::vector<Game*> candidates);
		std::vector<Game*> expand(Game* game, int a, int b);
		int backpropagate(Game* game, int player);
		int rollout(Game* game, int a, int b, int player, int depth);
		
		int alphabeta(Game* game, int a, int b, int startp, int maxdepth, int& best);
		int mt_sss(Game* game, int range, int player, int depth, int& best_move);
};

#endif
