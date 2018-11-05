/*
 * All necessary functions for alpha beta to communicate with the Hex
 * game, of which the code was written by Ali Mirsoleimani.
 * 
 * Own hashing implemented. Currently doesn't deal with collisions
 */
#include <stack>
#include <unordered_map>
#include <stdlib.h>
#include <climits>
#include <cmath>

#include "game/game.h"
#include "game/hex/HexState.h"

using namespace std;

namespace std
{
	template<> struct hash<HexGameState>
	{
		size_t operator()(const HexGameState& state) const noexcept
		{
			return state.get_hash();
		}
	};
}

struct StateInfo
{
	HexGameState hex_state;
	/**
	 * If we have a hash collision, next state is stored here
	 */
	StateInfo* next = NULL;
	/**
	 * Children of this state.
	 * If empty: either there are none or we need to calculate this
	 */
	vector<StateInfo*> children;
};

const int DIM = 2;

/** Vector mapping state hash to a state pointer */
unordered_map<size_t, StateInfo*> states;
/** Stack containing the current path through the game's state space */
stack<StateInfo*> history;
/** Number of possible moves from this state */
int nmoves;
/** Shows whether moves and nmoves have been calculated for the current
 *  state */
bool moves_set;
/** Maps states to their indices in the states vector */
//unordered_map<HexGameState, size_t> reverse_map;

/**
 * Calculate the child state of states[parent] after executing the given
 * move. If it has been previously calculated, return the index. If not,
 * append the state to the states vector and return the index.
 */
StateInfo* calc_child(StateInfo* parent, int move)
{
	HexGameState hex_state = parent->hex_state;
	hex_state.SetMove(move);
	size_t child_hash = hex_state.get_hash();
	StateInfo* child_state;
	
	auto child = states.find(child_hash);
	if(child == states.end())
	{
		child_state = new StateInfo();
		states[child_hash] = child_state;
		child_state->hex_state = hex_state;
	}
	else
		child_state = child->second;
	return child_state;
}

/**
 * Calculates the child states of the given parent for each of the given
 * moves. Stores these states in the states vector and returns the
 * indices of these children.
 */
vector<StateInfo*> calc_children(StateInfo* parent, vector<int> moves)
{
	vector<StateInfo*> children;
	for(int move : moves)
		children.push_back(calc_child(parent, move));
	return children;
}

/**
 * Calculate what moves are possible from the state at the top of the
 * move history stack. Also calculates the number of possible moves.
 * Sets moves_set to true.
 */
vector<int> Game::calc_moves()
{
	StateInfo* current = states[get_state()];
	vector<int> moves;
	if(current->hex_state.IsTerminal())
		nmoves = 0;
	else if(current->children.size() > 0)
		nmoves = current->children.size();
	else
	{
		nmoves = current->hex_state.GetMoves(moves);
		current->children = calc_children(states[get_state()], moves);
	}
	moves_set = true;
	return moves;
}

Game::Game(int seed)
{
	StateInfo* root = new StateInfo();
	HexGameState hex_state(DIM);
	hex_state.init_zobrist(seed);
	root->hex_state = hex_state;
	size_t root_hash = hex_state.get_hash();
	states = unordered_map<size_t, StateInfo*>();
	states[root_hash] = root;
	history.push(root);
	calc_moves();
}

Game::Game(){}

bool Game::game_over()
{
	return states[get_state()]->hex_state.IsTerminal();
}

int Game::get_nmoves()
{
	if(!moves_set)
		calc_moves();
	return nmoves;
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

vector<size_t> Game::get_child_states()
{
	if(!moves_set)
		calc_moves();
	vector<size_t> child_hashes;
	for(StateInfo* child : states[get_state()]->children)
		child_hashes.push_back(child->hex_state.get_hash());
	return child_hashes;
}

int Game::get_last_move()
{
	//TODO (or not. Is currently unused anyway...)
	return -1;
}

bool Game::is_max_node()
{
	return states[get_state()]->hex_state.GetPlyJM() == BLACK;//or WHITE?
}

void Game::set_root()
{
	size_t new_root = get_state();
	history = stack<StateInfo*>();
	history.push(states[new_root]);
}

size_t Game::get_state()
{
	return history.top()->hex_state.get_hash();
}

size_t Game::get_child_state(const int move)
{
	if(!moves_set)
		calc_moves();
	return states[get_state()]->children[move]->hex_state.get_hash();
}

size_t Game::do_move(const int move_id)
{
	size_t new_state = sim_move(move_id);
	set_root();
	return new_state;
}

size_t Game::sim_move(const int move_id)
{
	if(!moves_set)
		calc_moves();
	size_t new_state = get_child_state(move_id);
	move_to(new_state);
	return new_state;
}

size_t Game::undo_move()
{
	history.pop();
	moves_set = false;
	return history.top()->hex_state.get_hash();
}

void Game::move_to(const size_t state)
{
	bool is_child = false;
	StateInfo* new_state = states[state];
	for(StateInfo* child : states[get_state()]->children)
		if(child == new_state)
		{
			is_child = true;
			break;
		}
	
	if(!is_child)
		printf("State %zu is not a child of %zu!\n", state, get_state());
	else
		history.push(new_state);
		
	moves_set = false;
}

float Game::evaluate()
{
	HexGameState state = states[get_state()]->hex_state;
	int player = state.GetPlyJM();
	return state.GetResult(player);
}

int Game::get_winner()
{
	HexGameState root = states[get_state()]->hex_state;
	root.Print();
	if(!root.IsTerminal())
		return 0;
	else if(root.GetResult(BLACK) > 0)
		return 1;
	else if(root.GetResult(WHITE) > 0)
		return -1;
	else
		return 0;
}

void Game::print_board()
{
	states[get_state()]->hex_state.Print();
}
