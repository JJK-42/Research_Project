/*
 * Normal alpha beta algorithm
 * No rollouts
 * No parallelism
 */
#include <stdio.h>
#include <stdlib.h>

#include "algorithms/ab_simple.h"
#include "game/game.h"
#include "statistics/stats.h"

using namespace std;

const float MT_RANGE = 1.0;

float max(float x, float y)
{
	return x > y ? x : y;
}

Alphabeta::Alphabeta(){}

float Alphabeta::expand(Game game, float a, float b, int player, int depth, int& best)
{
	//printf("%i\n", depth);
	int nmoves = game.get_nmoves();
	float best_val = ALPHA;
	int child_best = -1;
	
	for(int i = 0; i < nmoves && a < b; i++)
	{
		game.sim_move(i);
		
		float val = -alphabeta(game, -b, -a, -player, depth - 1, child_best);
		
		game.undo_move();
		
		//printf("%zx val[%i]:%f\n", game.get_state(), i, val);
		
		if(val > best_val)
			best = i;
		
		best_val = max(best_val, val);
		a = max(a, val);
		
		if(best_val > b)
			break;
	}
	
	return best_val;
}

float Alphabeta::alphabeta(Game game, float a, float b, int player, int depth, int& best)
{
	size_t state = game.get_state();
	
	//printf("Alphabeta %f %f %zx\n", a, b, state);
	
	float lower = get_alpha(state);
	float upper = get_beta(state);
	float best_val;
	
	if(lower >= b)
		return lower;
	if(upper <= a)
		return upper;
	
	if(depth == 0 || game.game_over())
	{
		//printf("Evaluating: %zx\n", state);
		//game.print_board();
		stats.record_leaf(state);
		best_val = player * game.evaluate();
	}
	else
	{
		//printf("Expanding: %zx\n", state);
		//game.print_board();
		stats.record_expand(state);
		best_val = expand(game, a, b, player, depth, best);
	}
	
	if(best_val < b)
		store_beta(state, best_val);
	if(best_val > a)
		store_alpha(state, best_val);
	
	//printf("Returning %f\n", best_val);
	return best_val;
}

float Alphabeta::mt_sss(Game game, float range, int player, int depth, int& best_move)
{
	int root = game.get_state();
	float a_r = ALPHA;
	float b_r = BETA;
	while(a_r < b_r)
	{
		alphabeta(game, b_r - range, b_r, player, depth, best_move);
		a_r = get_alpha(root);
		b_r = get_beta(root);
	}
	return a_r;
}

int Alphabeta::run_algorithm(int argc, char** argv, Game game)
{
	stats.reset();
	
	int best_move;
	float winning_val;
	
	clear_bounds();
	
	bool print = true;
	if(argc > 1 && argv[1][0] == 'f')
		print = false;
	
	stats.start_timer();
	if(argc > 3 && argv[3][0] == 'M')
	{
		printf("Running MT_SSS* on alpha beta.\n");
		winning_val = mt_sss(game, MT_RANGE, 1, -1, best_move);
	}
	else
	{
		printf("Running alpha beta.\n");
		winning_val = alphabeta(game, ALPHA, BETA, 1, -1, best_move);
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
	
	return best_move;
}
