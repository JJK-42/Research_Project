#ifndef GAME_H
#define GAME_H

#include <vector>

/*
 * Contains the headers for the functions needed to interact with the
 * game. Like reading the game state and changing it.
 * 
 * Implementation will depend on the game and game code.
 */

class Game
{
	public:
		/**
		 * Constructor. Initialises the game.
		 * Should also create a new instance of the specific game.
		 */
		Game(const int seed);
		
		/**
		 * Constructor. Creates an uninitialised game.
		 */
		Game();
		
		~Game();
		
		size_t get_hash() const;
		
		/**
		 * Returns whether this game's game_state is equal to other's
		 * game_state.
		 */
		bool equals(const Game& other) const;
		
		/**
		 * Returns whether the game is finished.
		 */
		bool game_over();
		
		/**
		 * Clone this game and return a pointer to the clone.
		 */
		Game* clone() const;
		
		/**
		 * Returns the number of possible moves from the current state.
		 */
		int get_nmoves();
		
		/**
		 * Returns an estimation of the number of states.
		 * This value should be guaranteed to always be at least as high the
		 * actual number of states.
		 */
		int get_nstate_estimate();
		
		/**
		 * Returns whether the player who takes the next turn is the max
		 * player.
		 */
		bool is_max_node();
		
		std::vector<int> calc_moves();
		
		/**
		 * Executes the given move.
		 */
		void do_move(const int move_id);
		
		/**
		 * Rewinds the last move.
		 */
		void undo_move(int move_id);
		
		/**
		 * Evaluate the current game state.
		 */
		int evaluate(int max_a, int max_b);
		
		/**
		 * Check who wins the game. 1 if first player, -1 if second player
		 * 0 if tie. INT_MAX if game is unfinished.
		 */
		int get_winner();
		
		/**
		 * Prints the game board.
		 */
		void print_board();
		
	private:
		/**
		 * Pointer to the specific game state we're dealing with. It's up
		 * to the implementation of this header what this game will be.
		 */
		void* game_state;
};
#endif
