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

/**
 * Wrapper for thread return data.
 * @id id of the move
 * @val resulting value if the game is played out with this move
 * @stats statistics relevant to the calculation of this move
 */
struct Move_data
{
	int id;
	int val;
	
	Statistics* stats;
	
	Move_data(int move_id, int move_val, Statistics* statistics)
	{
		id = move_id;
		val = move_val;
		stats = statistics;
	}
	
	~Move_data()
	{
		delete stats;
	}
};

struct Thread_args
{
	RolPar* algorithm;
	Game* game;
	unsigned int seed;
	int thread_id;
	int max_depth;
	
	Thread_args(RolPar* alg, Game* g, unsigned int rand_seed, int tid, int mdepth)
	{
		algorithm = alg;
		game = g;
		seed = rand_seed;
		thread_id = tid;
		max_depth = mdepth;
	}
};

RolPar::RolPar(){}

RolPar::RolPar(Hashtable* htable) : Rollouts(htable){}

RolPar::~RolPar(){}

void RolPar::set_seed(unsigned int rand_seed)
{
	seed = rand_seed;
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
	int idx = rand_r(&seed) % candidates.size();
	return candidates[idx];
	
	//return candidates.front();
}

void* RolPar::new_thread(void* args)
{
	Thread_args* arguments = (Thread_args*)args;
	RolPar* alg = arguments->algorithm;
	Game* game = arguments->game;
	alg->set_seed(arguments->seed);
	
	int best_move = -1;
	int best_val = alg->alphabeta(game, ALPHA, BETA, 1, arguments->max_depth, best_move);
	
	Move_data* ret = new Move_data(best_move, best_val, alg->stats);
	delete game;
	delete alg;
	delete arguments;
	pthread_exit(ret);
}

/**
 * Runs the alpha beta by rollouts algorithm on whatever game is linked
 * to the game_interface.h header. Prints game seed, outcome and best
 * move for player 1 (max player).
 */
int RolPar::run_algorithm(int argc, char** argv, Game* game)
{
	stats->reset();
	
	bool print = false;
	if(argc > 1 && argv[1][0] == 't')
		print = true;
		
	int seed = 0;
	if(argc > 2)
		seed = atoi(argv[2]);
	
	int best_move;
	int winning_val = ALPHA;
	
	if(seed == -1)
		seed = time(NULL);
	srand(seed);
	
	int max_depth = -1;
	if(argc > 3)
		max_depth = atoi(argv[3]);
	printf("Max depth: %i\n", max_depth);
	
	int n_threads = 2;
	if(argc > 5)
		n_threads = atoi(argv[5]);
	if(n_threads > MAX_N_THREADS || n_threads <= 0)
		n_threads = MAX_N_THREADS;
	printf("N threads: %i\n", n_threads);
	
	stats->start_timer();
	if(argc > 4 && argv[4][0] == 'M')
	{
		printf("Running MT_SSS on rollouts alpha beta* with seed %i.\n", seed);
		winning_val = mt_sss(game, MT_RANGE, 1, -1, best_move);
	}
	else
	{
		printf("Running parallel rollouts alpha beta with seed %i.\n", seed);
		
		pthread_t thread[MAX_N_THREADS];
		for(int i = 0; i < n_threads; i++)
		{
			RolPar* algorithm = new RolPar(hash_table);
			Thread_args* args = new Thread_args(algorithm, game->clone(), seed + i, i, max_depth);
			
			int ret = pthread_create(&(thread[i]), NULL, new_thread, args);
			printf("Thread %i returned %i\n", i, ret);
		}
		Move_data* result[MAX_N_THREADS];
		
		for(int i = 0; i < n_threads; i++)
			pthread_join(thread[i], (void**)&result[i]);
		
		winning_val = hash_table->get_alpha(game);
		
		for(int i = 0; i < n_threads; i++)
		{
			//printf("Thread %i traversal:\n", i);
			//result[i]->stats->print_traversal();
			stats->merge(result[i]->stats);
			if(result[i]->val == winning_val)
				best_move = result[i]->id;
		}
		
		for(int i = 0; i < n_threads; i++)
			delete result[i];
	}
	stats->record_time();

	if(print)
	{
		printf("Outcome for player 1: %i\n", winning_val);
		printf("Best move: %i\n", best_move);
		if(game->get_nstate_estimate() < 128)
			stats->print_traversal();
		stats->print_stats();
		stats->print_ht();
	}
	
	//delete hash_table;
	
	vector<int> moves = game->calc_moves();
	return moves[best_move];
}
