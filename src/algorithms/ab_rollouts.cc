/*
 * Non-recursive alpha beta algorithm
 * With rollouts
 * No parallelism
 */
#include <float.h>
#include <stdio.h>
#include <stdlib.h>

#include "algorithms/ab_rollouts.h"
#include "game/game.h"
#include "statistics/stats.h"

using namespace std;

const int MT_RANGE = 10;

/**
 * If is_max, return whichever is greater of x and y
 * If not is_max, return whichever is smaller
 */
int minmax(int x, int y, bool is_max)
{
	if(is_max)
		return x > y ? x : y;
	else
		return x < y ? x : y;
}

Rollouts::Rollouts()
{
	stats = new Statistics();
}

/**
 * Evaluates the current game state and sets that state's alpha and beta
 * to this value. Also returns the value.
 */
int Rollouts::evaluate(Game* game)
{
	int val = game->evaluate(ALPHA, BETA);
	hash_table->swap_alpha(game, val);
	hash_table->swap_beta(game, val);
	stats->record_leaf(game->get_hash());
	return val;
}

/**
 * Selects a state to be expanded next.
 * (Currently simply the first state in the vector.)
 */
Game* Rollouts::select(vector<Game*> candidates)
{
	//For parallel AB, this could check whether the candidate is currently
	//being worked on. Pick the first available candidate that is not 
	//being worked on. If all are picked, just pick the first.
	
	//How do we classify "being worked on?" This could be if it is in a 
	//path of another process' rollout. But probably better to just
	//consider if the "evaluate" function is currently being called. Or if
	//it is at the top of another process' rollout stack.
	return candidates.front();
}

/**
 * Propagates alpha and beta forward to all children.
 * Then returns a vector of all children that are within the alpha-beta
 * window.
 */
vector<Game*> Rollouts::expand(Game* game, int a, int b)
{
	stats->record_expand(game->get_hash());
	
	vector<Game*> child_states;
	
	vector<int> moves = game->calc_moves();
	
	for(size_t i = 0; i < moves.size(); i++)
	{
		Game* child = game->clone();
		child->do_move(moves[i]);
		
		int achild = hash_table->get_alpha(child);
		if(a > achild)
		{
			achild = a;
			hash_table->swap_alpha(child, a);
		}
		
		int bchild = hash_table->get_beta(child);
		if(b > bchild)
		{
			bchild = b;
			hash_table->swap_beta(child, b);
		}
		
		if(achild < bchild)
			child_states.push_back(child);
		else
			delete child;
	}
	
	return child_states;
}

/**
 * Calculates a state's alpha and beta. Executed before returning one
 * recursive iteration of the rollout algorithm.
 */
int Rollouts::backpropagate(Game* game, int player)
{
	int best_move = -1;
	//size_t state = game.get_state();
	bool is_max = player > 0;
	
	int a_s;
	int b_s;
	if(is_max)
		a_s = b_s = ALPHA;
	else
		a_s = b_s = BETA;
	
	vector<int> moves = game->calc_moves();
	
	for(size_t i = 0; i < moves.size(); i++)
	{
		Game* child = game->clone();
		child->do_move(moves[i]);
		
		int a_c = hash_table->get_alpha(child);
		if((is_max && a_c > a_s) || (!is_max && a_c < a_s))
		{
			a_s = a_c;
			best_move = i;
		}
		
		int b_c = hash_table->get_beta(child);
		if((is_max && b_c > b_s) || (!is_max && b_c < b_s))
		{
			b_s = b_c;
			best_move = i;
		}
		
		delete child;
	}
	hash_table->swap_alpha(game, a_s);
	hash_table->swap_beta(game, b_s);
	return best_move;
}

/**
 * Rolls out a single path through the game tree.
 * Will never revisit a previous path due to its alpha-beta pruning
 * window.
 */
int Rollouts::rollout(Game* game, int a, int b, int player, int depth)
{
	//printf("%i\n", depth);
	int nmoves = game->get_nmoves();
	if(nmoves > 0 && depth != 0)
	{
		vector<Game*> candidates = expand(game, a, b);
		if(candidates.size() > 0)
		{
			Game* next = select(candidates);
			rollout(next, hash_table->get_alpha(next), hash_table->get_beta(next), -player, depth - 1);
			for(Game* g : candidates)
				delete g;
			candidates.clear();
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
int Rollouts::alphabeta(Game* game, int a, int b, int startp, int maxdepth, int& best)
{
	hash_table->swap_alpha(game, a);
	hash_table->swap_beta(game, b);
	
	while(a < b)
	{
		best = rollout(game, a, b, startp, maxdepth);
		a = hash_table->get_alpha(game);
		b = hash_table->get_beta(game);
	}
	
	return b;
}

int Rollouts::mt_sss(Game* game, int range, int player, int depth, int& best_move)
{
	while(hash_table->get_alpha(game) < hash_table->get_beta(game))
		alphabeta(game, hash_table->get_beta(game) - range, hash_table->get_beta(game), player, depth, best_move);
	return hash_table->get_alpha(game);
}

/**
 * Runs the alpha beta by rollouts algorithm on whatever game is linked
 * to the game.h header. Prints game seed, outcome and best
 * move for player 1 (max player).
 */
int Rollouts::run_algorithm(int argc, char** argv, Game game)
{
	stats->reset();
	
	int best_move = -1;
	int winning_val;
	
	clear_bounds();
	
	bool print = true;
	if(argc > 1 && argv[1][0] == 'f')
		print = false;
	
	int max_depth = -1;
	if(argc > 3)
		max_depth = atoi(argv[3]);
	else
		printf("WARNING: No max depth specified!\n");
	
	stats->start_timer();
	
	Game* g = game.clone();
	if(argc > 4 && argv[4][0] == 'M')
	{
		printf("Running MT_SSS* on rollouts alpha beta.\n");
		winning_val = mt_sss(g, MT_RANGE, 1, -1, best_move);
	}
	else
	{
		printf("Running rollouts alpha beta.\n");
		winning_val = alphabeta(g, ALPHA, BETA, 1, -1, best_move);
	}
	stats->record_time();
	
	if(print)
	{
		printf("Outcome for player 1: %i\n", winning_val - BETA / 2);
		printf("Best move: %i\n", best_move);
		
		if(game.get_nstate_estimate() < 128)
			stats->print_traversal();
		stats->print_stats();
		stats->print_ht();
	}
	
	vector<int> moves = game.calc_moves();
	return moves[best_move];
}
