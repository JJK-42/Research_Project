// *******************************************************************
//   (C) Copyright 2013 Leiden Institute of Advanced Computer Science
//   Universiteit Leiden
//   All Rights Reserved
// *******************************************************************
// Kunstmatige Intelligentie --- Jungle
// *******************************************************************
// FILE INFORMATION:
//   File:     main.cc (Depends on: move.h,
//                                  move_list.h,
//                                  position.h,
//                                  search.h,
//                                  types.h)
//   Author:   Jonathan K. Vis
//   Revision: 0.03a
//   Date:     2013/01/28
// *******************************************************************
// DESCRIPTION:
//   Hier begint het programma. De gebruiker kan tegen de AI spelen.
//   Let op: stdout wordt gebruikt voor message passing tussen twee
//   AIs (zie: contest.sh). Gebruik voor communicatie met de gebruiker
//   stdlog. En om te debuggen stderr (vaak een goed idee).
//   gebruik: ./(executable) white/black depth
//   waarbij white of black aangeeft met welke kleur de AI speelt en
//   depth aangeeft hoe diep (in plies) de AI moet zoeken.
//   2013/01/25 CHANGED: er kan nu zowel als gebruiker tegen de AI
//                       worden gespeeld als twee AIs tegen elkaar
//                       mbv een named pipe (zie: contest.sh).
//   2013/01/28 ADDED: get_move functie om twee gebruikers tegen
//                     elkaar te laten spelen.
// *******************************************************************

#include "move.h"
#include "move_list.h"
#include "position.h"
#include "search.h"
#include "types.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

// Deze variabele aanpassen naar een naam naar keuze
static char const* const ENGINE_NAME = "MyJungleChessEngine v1.0";

bool parse_args(int const         argc,
                char const* const argv[],
                bool             &my_turn,
                int              &depth)
{
  if (argc < 3)
  {
    cerr << "Usage: " << argv[0] << " white/black depth" << endl;
    return false;
  } // if
  if (argv[1][0] == 'b' || argv[1][0] == 'B')
  {
    my_turn = false;
  } // if
  else if (argv[1][0] == 'w' || argv[1][0] == 'W')
  {
    my_turn = true;
  } // if
  else
  {
    cerr << "Error in first argument: expected `white' or `black'"
         << endl;
    return false;
  } // else
  depth = atoi(argv[2]);
  if (depth <= 0)
  {
    cerr << "Error in second argument: depth is expected to be an "
         << "integer > 0" << endl;
    return false;
  } // if
  return true;
} // parse_args

Move get_move(void)
{
  Move move(NONE, CAPTURED, CAPTURED, NONE);
  cin >> move;
  cin.ignore(1024, '\n');
  return move;
} // get_move

void random_move(Position &position,
                 int const depth,
                 Move     &move)
{
  Move_list move_list;
  static_cast<void>(depth); // ignore depth
  position.generate_moves(move_list);
  move = move_list.move(rand() % move_list.size());
} // random_move

void play(bool my_turn, int const depth)
{
  Position position;
  Move move;
  position.initial();
  while (!position.is_won() &&
         !position.is_threefold_repetition() &&
         !position.no_more_moves() &&
         !position.last_turn())
  {
    if (my_turn)
    {
      move = Move(NONE, CAPTURED, CAPTURED, NONE);

      // De onderstaande functieaanroep veranderen naar negamax(...)
      // of alphabeta(...) of get_move() om twee gebruikers tegen
      // elkaar te laten spelen.
      random_move(position, depth, move);
      cout << (position.is_white_turn() ? "white/" : "black/")
           << position.turn() / 2 + 1 << "> ";
      cout << move << endl;
    } // if
    else
    {
      move = Move(NONE, CAPTURED, CAPTURED, NONE);
      while (!move.is_valid() || !position.validate_move(move))
      {
        clog << endl;
        position.draw_ASCII(clog);
        clog << (position.is_white_turn() ? "white/" : "black/")
             << position.turn() / 2 + 1 << "> ";
        move = get_move();
      } // while
    } // else
    position.do_move(move);
    my_turn = !my_turn;
  } // while
  position.draw_ASCII(clog);
  clog << "Game ended in a ";
  if (position.is_won())
  {
    if (position.is_white_turn())
    {
      clog << "black win." << endl;
      cout << "white/lost> 0-1" << endl;
      cout << "black/win> 0-1" << endl;
    } // if
    else
    {
      clog << "white win." << endl;
      cout << "white/win> 1-0" << endl;
      cout << "black/lost> 1-0" << endl;
    } // else
  } // if
  else
  {
    clog << "draw." << endl;
    cout << "white/draw> 1/2-1/2" << endl;
    cout << "black/draw> 1/2-1/2" << endl;
  } // if
  return;
} // play

int main(int argc, char* argv[])
{
  bool my_turn;
  int depth;
  srand(static_cast<unsigned int>(time(0)));
  if (!parse_args(argc, argv, my_turn, depth))
  {
    return 1;
  } // if
  clog << "Engine `" << ENGINE_NAME << "' started as "
       << (my_turn ? "white." : "black.") << endl;

  cin.rdbuf()->pubsetbuf(0, 0);
  clog.rdbuf()->pubsetbuf(0, 0);
  cout.rdbuf()->pubsetbuf(0, 0);

  play(my_turn, depth);
  return 0;
} // main

