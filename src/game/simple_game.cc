/* 
 * A simple game (directly implements the Game class in
 * game_interface.h) made for quickly testing whether a game playing
 * algorithm works as intended.
 */
#include <stdlib.h>
#include <cstdio>
#include <limits.h>

#include "game/game.h"

using namespace std;

const int NLEAFS = 1 << 4;//assumed to be a power of 2
const float VAL_RANGE = 64.0;

size_t game_state;
int turn; //store turn separately to speed up retrieval
float leafs[NLEAFS];

void print_leafs()
{
	if(NLEAFS > 64)
		return;
	
	printf("Leafs: ");
	for(int i = 0; i < NLEAFS; i++)
		printf("%f ", leafs[i]);
	printf("\n");
}

int calc_turn()
{
	int i;
	for(i = 0; (NLEAFS >> i) > 0; i++)
		if((game_state >> i) <= 1)
			return i;
	
	return 0;
}

Game::Game(const int seed)
{
	srand(seed);
	game_state = 1;
	for(int i = 0; i < NLEAFS; i++)
		leafs[i] = ((float) rand()) / ((float) RAND_MAX) * VAL_RANGE - VAL_RANGE / 2.0;
	
	print_leafs();
}

Game::Game(){}

vector<int> Game::calc_moves()
{
	vector<int> moves;
	moves[0] = 0;
	moves[1] = 1;
	return moves;
}

bool Game::is_max_node()
{
	return (turn & 1) == 0;
}

bool Game::game_over()
{
	return (NLEAFS >> turn) <= 1;
}

int Game::get_last_move()
{
	return game_state & 1;
}

int Game::get_nmoves()
{
	if(!game_over())
		return 2;
	else
		return 0;
}

int Game::get_nstate_estimate()
{
	return NLEAFS * 2;
}

vector<size_t> Game::get_child_states()
{
	vector<size_t> children;
	
	children[0] = do_move(0);
	undo_move();
	
	children[1] = do_move(1);
	undo_move();
	
	return children;
}

size_t Game::get_child_state(const int move_id)
{
	return (game_state << 1) + (move_id > 0);
}

size_t Game::get_state()
{
	return game_state;
}

void Game::set_root()
{
	
}

size_t Game::sim_move(const int move_id)
{
	game_state = game_state << 1;
	game_state += (move_id > 0);
	turn++;
	return game_state;
}

size_t Game::do_move(const int move_id)
{
	size_t new_state = sim_move(move_id);
	printf("%i\n", move_id);
	set_root();
	return new_state;
}

void Game::move_to(const size_t state)
{
	if(state == game_state)
		return;
	else if((state >> 1) == game_state)
		do_move(state & 1);
	else if((game_state >> 1) == state)
		undo_move();
	else
	{
		game_state = state;
		turn = calc_turn();
	}
}

size_t Game::undo_move()
{
	game_state = game_state >> 1;
	turn--;
	return game_state;
}

float Game::evaluate()
{
	if(game_over())
		return leafs[game_state & (NLEAFS - 1)] > 0 ? 1.0 : -1.0;//only works if NLEAFS is power of 2
	
	printf("WARNING: Evaluation of internal node not implemented!\n");
	return 0.0;
}

int Game::get_winner()
{
	if(!game_over())
		return INT_MAX;
	else
	{
		float res = evaluate();
		printf("Result: %f\n", res);
		if(res > 0)
			return 1;
		else if(res < 0)
			return -1;
		else
			return 0;
	}
}

void Game::print_board()
{
	printf(" ");
}
