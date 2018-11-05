#include <stdlib.h>
#include <stdio.h>

#include "game/game.h"
#include "algorithms/ab_simple.h"

using namespace std;

int main(int argc, char** argv)
{
	Alphabeta ab;
	int seed = 0;
	if(argc > 2)
		seed = atoi(argv[2]);
	Game* game = new Game(seed);
	ab.run_algorithm(argc, argv, *game);
	return 0;
}
