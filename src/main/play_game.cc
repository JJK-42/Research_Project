#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define __MAIN__

#include "play_game.h"
#include "algorithms/algorithm.h"
#include "algorithms/ab_rollouts.h"
#include "algorithms/ab_simple.h"
#include "game/game.h"

using namespace std;

Game* game;
vector<Algorithm*> players;

void print_win_message(int winner)
{
	if(winner > 0)
		printf("Black has won the game!\n");
	else if(winner < 0)
		printf("White has won the game!\n");
	else
		printf("Tie!\n");
}

void play_game(int argc, char** argv)
{
	int player = 0;
	int n_players = players.size();
	while(!game->game_over())
	{
		game->print_board();
		int move = get_move(argc, argv, player);
		game->do_move(move);
		player = (player + 1) % n_players;
	}
}

int human_move()
{
	int move_id;
	printf("Choose move: ");
	cin >> move_id;
	return move_id;
}

int get_move(int argc, char** argv, int player)
{
	if(players[player] == NULL)
		return human_move();
	else
	{
		int move = players[player]->run_algorithm(argc, argv, game);
		players[player]->clear_bounds();
		return move;
	}
}

int main(int argc, char** argv)
{
	int seed = 0;
	if(argc > 2)
		seed = atoi(argv[2]);
	
	printf("Starting game with seed %i.\n", seed);
	
	game = new Game(seed);
	
	for(int i = 4; i < argc; i++)
	{
		printf("Adding player: %s\n", argv[i]);
		if(argv[i][0] == 'r')
		{
			Rollouts* r = new Rollouts();
			players.push_back(r);
		}
		else if(argv[i][0] == 's' || argv[i][0] == 'a')
		{
			Alphabeta* a = new Alphabeta();
			players.push_back(a);
		}
		else
			players.push_back(NULL);
	}
	if(argc < 5)
		players.push_back(NULL);
		
	for(Algorithm* player : players)
		if(player == NULL)
			printf("Human player\n");
		else
			printf("AI player\n");
		
	play_game(argc, argv);
	print_win_message(game->get_winner());
	
	//This removed a memory leak for some reason. Weird.
	//The actual problem might lie elsewhere...
	for(Algorithm* player : players)
		if(player != NULL)
			delete player;
	
	return 0;
}
