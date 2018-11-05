/*
 * Parallel rollouts based alpha beta algorithm
 */
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <utility>

#include "algorithms/ab_rollouts_parallel.h"
#include "algorithms/hash_table.h"
#include "game/game.h"
#include "statistics/stats.h"

using namespace std;

const float MT_RANGE = 1.0;
const int N_THREADS = 2;

/**
 * If is_max, return whichever is greater of x and y
 * If not is_max, return whichever is smaller
 */
float minmax(float x, float y, bool is_max)
{
	if(is_max)
		return x > y ? x : y;
	else
		return x < y ? x : y;
}

RolPar::RolPar(){}

float RolPar::get_alpha(int state)
{
	float res = Algorithm::get_alpha(state);
	return res;
}

float RolPar::get_beta(int state)
{
	float res = Algorithm::get_beta(state);
	return res;
}

void RolPar::store_alpha(int state, float val)
{
	Algorithm::store_alpha(state, val);
}

void RolPar::store_beta(int state, float val)
{
	Algorithm::store_beta(state, val);
}

/**
 * Evaluates the current game state and sets that state's alpha and beta
 * to this value. Also returns the value.
 */
float RolPar::evaluate(Game game)
{
	int node = game.get_state();
	float val = game.evaluate();
	store_alpha(node, val);
	store_beta(node, val);
	stats.record_leaf(node);
	return val;
}

/**
 * Selects a state to be expanded next.
 * (Currently a random state in the vector.)
 */
int RolPar::select(vector<int> candidates)
{
	//For parallel AB, this could check whether the candidate is currently
	//being worked on. Pick the first available candidate that is not 
	//being worked on. If all are picked, just pick the first.
	
	//How do we classify "being worked on?" This could be if it is in a 
	//path of another process' rollout. But probably better to just
	//consider if the "evaluate" function is currently being called. Or if
	//it is at the top of another process' rollout stack.
	
	//Currently selects a random candidate.
	int idx = rand() % candidates.size();
	return candidates[idx];
	
	//return candidates.front();
}

/**
 * Propagates alpha and beta forward to all children.
 * Then returns a vector of all children that are within the alpha-beta
 * window.
 */
vector<int> RolPar::expand(Game game, float a, float b)
{
	stats.record_expand(game.get_state());
	
	vector<int> child_states;
	
	for(int i = 0; i < game.get_nmoves(); i++)
	{
		int child = game.get_child_state(i);
		
		float a_c = get_alpha(child);
		if(a > a_c)
		{
			store_alpha(child, a);
			a_c = a;
		}
		
		float b_c = get_beta(child);
		if(b < b_c)
		{
			store_beta(child, b);
			b_c = b;
		}
		
		if(a_c < b_c)
			child_states.push_back(child);
	}
	return child_states;
}

/**
 * Calculates a state's alpha and beta. Executed before returning one
 * recursive iteration of the rollout algorithm.
 */
int RolPar::backpropagate(Game game, int player)
{
	int best_move = -1;
	int state = game.get_state();
	bool is_max = player > 0;
	
	float a_s;
	float b_s;
	if(is_max)
		a_s = b_s = ALPHA;
	else
		a_s = b_s = BETA;
	
	for(int i = 0; i < game.get_nmoves(); i++)
	{
		int child = game.get_child_state(i);
		
		float a_c = get_alpha(child);
		if((is_max && a_c > a_s) || (!is_max && a_c < a_s))
		{
			a_s = a_c;
			best_move = i;
		}
		
		float b_c = get_beta(child);
		if((is_max && b_c > b_s) || (!is_max && b_c < b_s))
		{
			b_s = b_c;
			best_move = i;
		}
	}
	store_alpha(state, a_s);
	store_beta(state, b_s);
	return best_move;
}

/**
 * Rolls out a single path through the game tree.
 * Will never revisit a previous path due to its alpha-beta pruning
 * window.
 */
int RolPar::rollout(Game game, float a, float b, int player)
{
	int nmoves = game.get_nmoves();
	if(nmoves > 0)
	{
		vector<int> candidates = expand(game, a, b);
		if(candidates.size() > 0)
		{
			int next = select(candidates);
			game.move_to(next);
			rollout(game, get_alpha(next), get_beta(next), -player);
			game.undo_move();
		}
		return backpropagate(game, player);
	}
	else
		evaluate(game);
	return -1;
}

/**
 * Alpha beta with rollouts.
 * Calls rollouts until a solution has been found.
 */
float RolPar::alphabeta(Game game, float a, float b, int startp, int maxdepth, int& best)
{
	int root = game.get_state();
	
	store_alpha(root, a);
	store_beta(root, b);
	
	float a_r = a;
	float b_r = b;
	
	while(a_r < b_r)
	{
		best = rollout(game, a_r, b_r, startp);
		a_r = get_alpha(root);
		b_r = get_beta(root);
	}
	
	return b_r;
}

void* RolPar::run_thread(void* ptr)
{
	Game game = *(Game*)ptr;
	int best_move;
	float best_val = alphabeta(game, ALPHA, BETA, 1, -1, best_move);
	pthread_exit(&make_pair(best_val, best_move));
}

float RolPar::mt_sss(Game game, float range, int player, int depth, int& best_move)
{
	//TODO: adapt for parallelism
	int root = game.get_state();
	while(get_alpha(root) < get_beta(root))
		alphabeta(game, get_beta(root) - range, get_beta(root), player, depth, best_move);
	return get_alpha(root);
}

/**
 * Runs the alpha beta by rollouts algorithm on whatever game is linked
 * to the game_interface.h header. Prints game seed, outcome and best
 * move for player 1 (max player).
 */
int RolPar::run_algorithm(int argc, char** argv, Game game)
{
	stats.reset();
	
	bool print = true;
	if(argc > 1 && argv[1][0] == 'f')
		print = false;
		
	int seed = 0;
	if(argc > 2)
		seed = atoi(argv[2]);
	
	int best_move;
	float winning_val = ALPHA;
	
	srand(seed);
	
	stats.start_timer();
	if(argc > 3)
	{
		printf("Running MT_SSS on rollouts alpha beta* with seed %i.\n", seed);
		winning_val = mt_sss(game, MT_RANGE, 1, -1, best_move);
	}
	else
	{
		printf("Running rollouts alpha beta with seed %i.\n", seed);
		
		pthread_t thread[N_THREADS];
		for(int i = 0; i < N_THREADS; i++)
		{
			int ret = pthread_create(&(thread[i]), NULL, run_thread, NULL);
			printf("pthread_create returned %i for thread %i.\n", ret, i);
		}
		
		winning_val = get_alpha(game);
		pair<float, int> result[N_THREADS];
		
		for(int i = 0; i < N_THREADS; i++)
			pthread_join(thread[i], &(result[i]));
		
		for(int i = 0; i < N_THREADS; i++)
			if(result[i]->first == winning_val)
				best_move = result[i]->second;
	}
	stats.record_time();

	if(print)
	{
		printf("Outcome for player 1: %f\n", winning_val);
		printf("Best move: %i\n", best_move);
		if(game.get_nstate_estimate() < 128)
			stats.print_traversal();
		stats.print_stats();
	}
	
	return 0;
}

#ifndef __MAIN__
#define __MAIN__

int main(int argc, char** argv)
{
	Rollouts algorithm;
	
	int seed = 0;
	if(argc > 2)
		seed = atoi(argv[2]);
	
	Game game(seed);
	return algorithm.run_algorithm(argc, argv, game);
}

#endif
