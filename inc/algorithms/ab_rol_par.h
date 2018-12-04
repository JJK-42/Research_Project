#ifndef AB_ROL_PAR_H
#define AB_ROL_PAR_H

//#include "algorithms/hash_table.h"
#include "algorithms/ab_rollouts.h"

const int MAX_N_THREADS = 128;

class RolPar : public Rollouts
{
	public:
		RolPar();
		RolPar(Hashtable* ht);
		~RolPar();
		
		int run_algorithm(int argc, char** argv, Game* game);
		
		void set_seed(unsigned int seed);
		
	protected:
		//static thread_local int thread_id;
		unsigned int seed;
		
		Game* select(std::vector<Game*> candidates);
		std::vector<Game*> expand(Game* game, int a, int b);
		
		static void* new_thread(void* args);
};

#endif
