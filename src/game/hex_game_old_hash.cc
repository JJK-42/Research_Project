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
	HexGameState state;
	/**
	 * Children of this state.
	 * If empty: either there are none or we need to calculate this
	 */
	vector<size_t> children;
};

const int DIM = 4;

/** Vector mapping state id to an actual state */
vector<StateInfo> states;
/** Stack containing the current path through the game's state space */
stack<size_t> history;
/** Number of possible moves from this state */
int nmoves;
/** Shows whether moves and nmoves have been calculated for the current
 *  state */
bool moves_set;
/** Maps states to their indices in the states vector */
unordered_map<HexGameState, size_t> reverse_map;

/**
 * Calculate the child state of states[parent] after executing the given
 * move. If it has been previously calculated, return the index. If not,
 * append the state to the states vector and return the index.
 */
size_t calc_child(size_t parent, int move)
{
	StateInfo new_state;
	new_state.state = states[parent].state;
	new_state.state.SetMove(move);
	auto child = reverse_map.find(new_state.state);
	if(child == reverse_map.end())
	{
		size_t index = states.size();
		states.push_back(new_state);
		reverse_map[new_state.state] = index;
		//new_state.state.Print();
		//printf("%zx\n", new_state.state.get_hash());
		return index;
	}
	else
		return child->second;
}

/**
 * Calculates the child states of the given parent for each of the given
 * moves. Stores these states in the states vector and returns the
 * indices of these children.
 */
vector<size_t> calc_children(size_t parent, vector<int> moves)
{
	vector<size_t> children;
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
	get_state();
	StateInfo current = states[get_state()];
	vector<int> moves;
	if(current.state.IsTerminal())
		nmoves = 0;
	else if(current.children.size() > 0)
		nmoves = current.children.size();
	else
	{
		nmoves = current.state.GetMoves(moves);
		current.children = calc_children(get_state(), moves);
		states[get_state()] = current;
	}
	moves_set = true;
	return moves;
}

Game::Game(int seed)
{
	StateInfo stateInfo;
	HexGameState hexState(DIM);
	hexState.init_zobrist(seed);
	stateInfo.state = hexState;
	states.clear();
	states.push_back(stateInfo);
	history.push(0);
	calc_moves();
}

Game::Game(){}

bool Game::game_over()
{
	return states[get_state()].state.IsTerminal();
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
	return states[get_state()].children;
}

int Game::get_last_move()
{
	//TODO
	return -1;
}

bool Game::is_max_node()
{
	return states[get_state()].state.GetPlyJM() == BLACK;//or WHITE?
}

void Game::set_root()
{
	size_t new_root = get_state();
	history = stack<size_t>();
	history.push(new_root);
}

size_t Game::get_state()
{
	return history.top();
}

size_t Game::get_child_state(const int move)
{
	if(!moves_set)
		calc_moves();
	return states[get_state()].children[move];
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
	return history.top();
}

void Game::move_to(const size_t state)
{
	bool is_child = false;
	for(size_t child : states[get_state()].children)
		if(child == state)
		{
			is_child = true;
			break;
		}
	
	if(!is_child)
		printf("State %zu is not a child of %zu!\n", state, get_state());
	else
		history.push(state);
		
	moves_set = false;
}

float Game::evaluate()
{
	HexGameState state = states[get_state()].state;
	//printf("Eval: %zx => %f\n", state.get_hash(), state.GetResult(BLACK) > 0 ? 1.0 : (state.GetResult(WHITE) > 0 ? -1.0 : 0.0));
	if(!state.IsTerminal())
		printf("WARNING: Evaluating non terminal state!\n");
	if(state.GetResult(BLACK) > 0)
		return 1.0;
	else if(state.GetResult(WHITE) > 0)
		return -1.0;
	else
		return 0.0;
}

int Game::get_winner()
{
	HexGameState root = states[get_state()].state;
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
	states[get_state()].state.Print();
}
