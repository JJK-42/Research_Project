#ifndef AB_ROL_PAR_H
#define AB_ROL_PAR_H

#include "algorithms/hash_table.h"
#include "algorithms/algorithm.h"

class RolPar : public Algorithm
{
	public:
		RolPar();
		
		int run_algorithm(int argc, char** argv, Game game);
		
	protected:
		//float maxnode(Game game, float a, float b, int depth, int& best);
		//float minnode(Game game, float a, float b, int depth, int& best);
		
		float expand(Game game, float a, float b, int player, int depth, int& best);
		
		float alphabeta(Game game, float a, float b, int player, int depth, int& best);
		float mt_sss(Game game, float range, int player, int depth, int& best_move);
};

#endif
