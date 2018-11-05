#ifndef PLAY_GAME_H
#define PLAY_GAME_H

/**
 * Play a full game using the rollout method
 * Parameters:
 * - TODO
 */
void play_game(int argc, char** argv);

int human_move();

/**
 * Calculates the next move of a given player or checks input
 */
int get_move(int argc, char** argv, int player);

void print_win_message(int winner);

#endif
