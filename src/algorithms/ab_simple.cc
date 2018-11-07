/*
 * Normal alpha beta algorithm
 * No rollouts
 * No parallelism
 */
#include <stdio.h>
#include <stdlib.h>

#include "algorithms/ab_simple.h"
#include "algorithms/hash_table.h"
#include "game/game.h"
#include "statistics/stats.h"

using namespace std;

const int MT_RANGE = 10;

int max(int x, int y)
{
	return x > y ? x : y;
}

Alphabeta::Alphabeta()
{
	stats = new Statistics();
}

int Alphabeta::expand(Game* game, int a, int b, int player, int depth, int& best)
{
	int nmoves = game->get_nmoves();
	int best_val = ALPHA;
	int child_best = -1;
	vector<int> moves = game->calc_moves();
	
	for(int i = 0; i < nmoves && a < b; i++)
	{
		Game* child = game->clone();
		child->do_move(moves[i]);
		
		int val = BETA - alphabeta(child, BETA - b + ALPHA, BETA - a + ALPHA, -player, depth - 1, child_best) + ALPHA;
		
		delete child;
		
		if(val >= best_val || best < 0)
			best = i;
		
		best_val = max(best_val, val);
		a = max(a, val);
		
		if(best_val > b)
			break;
	}
	
	return best_val;
}

int Alphabeta::alphabeta(Game* game, int a, int b, int player, int depth, int& best)
{
	int lower = hash_table->get_alpha(game);
	int upper = hash_table->get_beta(game);
	int best_val;
	
	if(lower >= b)
		return lower;
	if(upper <= a)
		return upper;
	
	if(depth == 0 || game->game_over())
	{
		stats->record_leaf(game->get_hash());
		if(player > 0)
			best_val = game->evaluate(ALPHA, BETA);
		else
			best_val = BETA - game->evaluate(ALPHA, BETA) + ALPHA;
	}
	else
	{
		stats->record_expand(game->get_hash());
		best_val = expand(game, a, b, player, depth, best);
	}
	
	hash_table->increase_alpha(game, best_val, a);
	hash_table->increase_beta(game, best_val, b);
	
	return best_val;
}

int Alphabeta::mt_sss(Game* game, int range, int player, int depth, int& best_move)
{
	int a_r = ALPHA;
	int b_r = BETA;
	while(a_r < b_r)
	{
		alphabeta(game, b_r - range, b_r, player, depth, best_move);
		a_r = hash_table->get_alpha(game);
		b_r = hash_table->get_beta(game);
	}
	return a_r;
}

int Alphabeta::run_algorithm(int argc, char** argv, Game* game)
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
	
	Game* g = game->clone();
	if(argc > 4 && argv[4][0] == 'M')
	{
		printf("Running MT_SSS* on alpha beta.\n");
		winning_val = mt_sss(g, MT_RANGE, 1, max_depth, best_move);
	}
	else
	{
		printf("Running alpha beta with max depth = %i.\n", max_depth);
		winning_val = alphabeta(g, ALPHA, BETA, 1, max_depth, best_move);
	}
	stats->record_time();
	
	if(print)
	{
		printf("Outcome for player 1: %i\n", winning_val - BETA / 2);
		printf("Best move: %i\n", best_move);
		
		if(game->get_nstate_estimate() < 128)
			stats->print_traversal();
		stats->print_stats();
		stats->print_ht();
	}
	
	vector<int> moves = game->calc_moves();
	return moves[best_move];
}
