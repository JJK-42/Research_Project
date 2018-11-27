/*
 * Parallel rollouts based alpha beta algorithm
 */
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <utility>

#include "algorithms/ab_rol_par.h"
#include "algorithms/hash_table.h"
#include "game/game.h"
#include "statistics/stats.h"

using namespace std;

const int MT_RANGE = 10;
const int N_THREADS = 2;

/**
 * Wrapper for thread return data.
 * @id id of the move
 * @val resulting value if the game is played out with this move
 */
struct Move_data
{
	int id;
	int val;
	
	Move_data(int move_id, int move_val)
	{
		id = move_id;
		val = move_val;
	}
};

struct Thread_args
{
	RolPar* algorithm;
	Game* game;
	unsigned int seedp;
	int thread_id;
	
	Thread_args(RolPar* alg, Game* g, unsigned int seedpointer, int tid)
	{
		algorithm = alg;
		game = g;
		seedp = seedpointer;
		thread_id = tid;
	}
};

RolPar::RolPar(){}

RolPar::RolPar(Hashtable* htable) : Rollouts(htable){}

RolPar::~RolPar(){}

void RolPar::set_seed(unsigned int seedpointer)
{
	seedp = seedpointer;
}

/**
 * Selects a state to be expanded next.
 * (Currently a random state in the vector.)
 */
Game* RolPar::select(vector<Game*> candidates)
{
	//For parallel AB, this could check whether the candidate is currently
	//being worked on. Pick the first available candidate that is not 
	//being worked on. If all are picked, just pick the first.
	
	//How do we classify "being worked on?" This could be if it is in a 
	//path of another process' rollout. But probably better to just
	//consider if the "evaluate" function is currently being called. Or if
	//it is at the top of another process' rollout stack.
	
	//Currently selects a random candidate.
	int idx = rand_r(&seedp) % candidates.size();
	return candidates[idx];
	
	//return candidates.front();
}

void* RolPar::new_thread(void* args)
{
	Thread_args* arguments = (Thread_args*)args;
	RolPar* alg = arguments->algorithm;
	Game* game = arguments->game;
	alg->set_seed(arguments->seedp);
	
	int best_move = -1;
	int best_val = alg->alphabeta(game, ALPHA, BETA, 1, -1, best_move);
	
	printf("Best: move %i, val %i\n", best_move, best_val);
	
	Move_data* ret = new Move_data(best_move, best_val);
	delete game;
	pthread_exit(&ret);
}

/**
 * Runs the alpha beta by rollouts algorithm on whatever game is linked
 * to the game_interface.h header. Prints game seed, outcome and best
 * move for player 1 (max player).
 */
int RolPar::run_algorithm(int argc, char** argv, Game* game)
{
	stats->reset();
	
	bool print = true;
	if(argc > 1 && argv[1][0] == 'f')
		print = false;
		
	int seed = 0;
	if(argc > 2)
		seed = atoi(argv[2]);
	
	int best_move;
	int winning_val = ALPHA;
	
	if(seed == -1)
		seed = time(NULL);
	srand(seed);
	
	stats->start_timer();
	if(argc > 3)
	{
		printf("Running MT_SSS on rollouts alpha beta* with seed %i.\n", seed);
		winning_val = mt_sss(game, MT_RANGE, 1, -1, best_move);
	}
	else
	{
		printf("Running rollouts alpha beta with seed %i.\n", seed);
		
		pthread_t thread[N_THREADS];//TODO: replace n threads with cmd line arg
		for(int i = 0; i < N_THREADS; i++)
		{
			RolPar* algorithm = new RolPar(hash_table);
			Thread_args* args = new Thread_args(algorithm, game->clone(), seed + i, i);
			
			int ret = pthread_create(&(thread[i]), NULL, new_thread, &args);
			printf("pthread_create returned %i for thread %i.\n", ret, i);
		}
		
		winning_val = hash_table->get_alpha(game);
		Move_data* result[N_THREADS];
		
		for(int i = 0; i < N_THREADS; i++)
			pthread_join(thread[i], (void**)result[i]);
		
		for(int i = 0; i < N_THREADS; i++)
		{
			printf("Thread %i returned: move %i, val %i\n", i, result[i]->id, result[i]->val);
			if(result[i]->val == winning_val)
				best_move = result[i]->id;
		}
	}
	stats->record_time();

	if(print)
	{
		printf("Outcome for player 1: %i\n", winning_val);
		printf("Best move: %i\n", best_move);
		if(game->get_nstate_estimate() < 128)
			stats->print_traversal();
		stats->print_stats();
	}
	
	return 0;
}
