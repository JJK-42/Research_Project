/*
 * All necessary functions for alpha beta to communicate with the Hex
 * game, of which the code was written by Ali Mirsoleimani.
 * 
 * Note: this version stores all previously evaluated states in a map
 */
#include <stack>
#include <unordered_map>
#include <stdlib.h>
#include <climits>
#include <cmath>

#include "game/game.h"
#include "game/hex/HexState.h"

using namespace std;

const int DIM = 3;
const int HOR = 1;
const int VER = DIM;

Game::Game(int seed)
{
	HexGameState* hex_state = new HexGameState(DIM);
	if(!HexGameState::hash_is_initialised())
		HexGameState::init_zobrist(seed);
	game_state = hex_state;
}

Game::Game()
{
	HexGameState* hex_state = new HexGameState(DIM);
	if(!HexGameState::hash_is_initialised())
		HexGameState::init_zobrist(0);
	game_state = hex_state;
}

Game::Game(void* state)
{
	game_state = (HexGameState*)state;
}

Game::~Game()
{
	delete (HexGameState*)game_state;
}

size_t Game::get_hash() const
{
	return ((HexGameState*)game_state)->get_hash();
}

bool Game::game_over()
{
	return ((HexGameState*)game_state)->IsTerminal();
}

Game* Game::clone() const
{
	HexGameState* other_state = new HexGameState(*(HexGameState*)game_state);
	Game* other = new Game(other_state);
	return other;
}

bool Game::equals(const Game& other) const
{
	HexGameState this_state = *(HexGameState*)game_state;
	HexGameState other_state = *(HexGameState*)(other.game_state);
	return this_state == other_state;
}

/**
 * Calculate what moves are possible from the state at the top of the
 * move history stack. Also calculates the number of possible moves.
 * Sets moves_set to true.
 */
vector<int> Game::calc_moves()
{
	vector<int> moves;
	((HexGameState*)game_state)->GetMoves(moves);
	return moves;
}

int Game::get_nmoves()
{
	vector<int> moves;
	return ((HexGameState*)game_state)->GetMoves(moves);
}

int Game::get_nstate_estimate()
{
	//Only actually used to check whether it's realistic to display all
	//alphas and betas.
	if(DIM > 3)
		return INT_MAX;
	else
		return pow(N_PIECES + 1, DIM * DIM);
}

bool Game::is_max_node()
{
	return ((HexGameState*)game_state)->GetPlyJM() == BLACK;//or WHITE?
}

void Game::do_move(const int move_id)
{
	((HexGameState*)game_state)->SetMove(move_id);
}

void Game::undo_move(int move_id)
{
	//TODO?
	((HexGameState*)game_state)->UndoMove(move_id);
}

int evaluate_position(HexGameState* state, int pos, int player, int dir, int max_a, int max_b)
{
	int score = 0;
	int unit = 1;//(float)(max_b - max_a) / (float)(DIM * DIM);
	if(state->get_val(pos) == player)
	{
		bool left = pos % DIM == 0;
		bool right = pos % DIM == DIM - 1;
		bool top = pos < DIM;
		bool bottom = pos >= DIM * DIM - DIM;
		
		if((dir == VER && (top || bottom))
		 || (dir == HOR && (left || right)))
			score += unit;
		if((!left && !top)
		 && state->get_val(pos - DIM - 1) == player)//pos - DIM - 1 -> top left
			score += unit;
		if((!right && !bottom)
		 && state->get_val(pos + DIM + 1) == player)//pos + DIM + 1 -> bottom right
			score += unit;
		if((dir == HOR && !left)
		 && state->get_val(pos - 1) == player)//pos - 1 -> left
			score += unit;
		if((dir == HOR && !right)
		 && state->get_val(pos + 1) == player)//pos + 1 -> right
			score += unit;
		if((dir == VER && !top)
		 && state->get_val(pos - DIM) == player)//pos - DIM -> top
			score += unit;
		if((dir == VER && !bottom)
		 && state->get_val(pos + DIM) == player)//pos + DIM -> bottom
			score += unit;
	}
	return score;
}

int evaluate_internal(HexGameState* state, int max_a, int max_b)
{
	int game_val = (float)max_b / 2.0 + (float)max_a / 2.0;
	int bscore = 0;
	int wscore = 0;
	
	for(int i = 0; i < DIM * DIM; i++)
	{
		bscore += evaluate_position(state, i, BLACK, HOR, max_a, max_b);
		wscore += evaluate_position(state, i, WHITE, VER, max_a, max_b);
	}
	
	return game_val - bscore + wscore;
}

int Game::evaluate(int max_a, int max_b)
{
	HexGameState* state = (HexGameState*)game_state;
	
	if(!state->IsTerminal())
		return evaluate_internal(state, max_a, max_b);
	if(state->GetResult(BLACK) > 0)
		return max_b;
	else if(state->GetResult(WHITE) > 0)
		return max_a;
	else
		return (float)max_b / 2.0 + (float)max_a / 2.0;
}

int Game::get_winner()
{
	HexGameState* state = (HexGameState*)game_state;
	
	print_board();
	
	if(!state->IsTerminal())
		return 0;
	else if(state->GetResult(BLACK) > 0)
		return 1;
	else if(state->GetResult(WHITE) > 0)
		return -1;
	else
		return 0;
}

void Game::print_board()
{
	((HexGameState*)game_state)->Print();
}
