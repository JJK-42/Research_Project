#ifndef AB_ROL_PAR_H
#define AB_ROL_PAR_H

//#include "algorithms/hash_table.h"
#include "algorithms/ab_rollouts.h"

class RolPar : public Rollouts
{
	public:
		RolPar();
		~RolPar();
		
		int run_algorithm(int argc, char** argv, Game* game);
		
	protected:
		Game* select(std::vector<Game*> candidates);
		std::vector<Game*> expand(Game* game, int a, int b);
		
		static void* new_thread(void* args);
		
		//int alphabeta(Game* game, float a, float b, int player, int depth, int& best);
		//int mt_sss(Game* game, float range, int player, int depth, int& best_move);
};

#endif
