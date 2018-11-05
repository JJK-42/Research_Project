// *******************************************************************
//   (C) Copyright 2013 Leiden Institute of Advanced Computer Science
//   Universiteit Leiden
//   All Rights Reserved
// *******************************************************************
// Kunstmatige Intelligentie --- Jungle
// *******************************************************************
// FILE INFORMATION:
//   File:     position.cc (Depends on: position.h)
//   Author:   Jonathan K. Vis
//   Revision: 0.03a
//   Date:     2013/02/05
// *******************************************************************
// DESCRIPTION:
//   Implementatie van de positie klasse.
//   2013/01/24 ADDED: referentie van de stukken naast het bord bij
//                     afdrukken (draw_ASCII)
//   2013/01/24 FIXED: warning DEV-C++/Windows in is_white_turn:
//                     unsigned vs signed comparision
//   2013/02/05 ADDED: de spelconfiguratie wordt ge\"evalueerd bij het
//                     afdrukken (draw_ASCII)
//   2013/02/05 ADDED: een drietal functies met toegang tot de
//                     private members mbt de plaats van de stukken op
//                     het bord
// *******************************************************************

#include "position.h"

// *******************************************************************
// CONSTRUCTORS (2)
// *******************************************************************
Position::Position(void)
{
  // intentionally empty
} // Position::Position

Position::Position(Position const &position)
{
  copy(position);
} // Position::Position

// *******************************************************************
// Genereren, doen en ongedaan maken van zetten (ook null moves). (5)
// *******************************************************************
void Position::generate_moves(Move_list &move_list)
{
  uint32_t i = _turn & 1;
  for (uint32_t d = STEP_NORTH; d <= STEP_WEST; d++)
  {
    uint32_t to;
    if (can_move(i, _piece[i], d, to) &&
        (can_capture(i, _board[to]) || is_trapped(_board[to], to) ||
         (!is_water(_piece[i]) && is_elephant(_board[to]) &&
          is_enemy(i, _board[to]))))
    {
      Move move = Move(i, _piece[i], to, _board[to]);
      do_move(move);
      move.set_value(evaluate());
      undo_move(move);
      move_list.add(move);
    } // if
  } // for
  for (i += 2; i < static_cast<uint32_t>(PIECES); i += 2)
  {
    for (uint32_t d = STEP_NORTH; d <= STEP_WEST; d++)
    {
      uint32_t to;
      if (can_move(i, _piece[i], d, to) &&
          (can_capture(i, _board[to]) || is_trapped(_board[to], to)))
      {
        Move move = Move(i, _piece[i], to, _board[to]);
        do_move(move);
        move.set_value(evaluate());
        undo_move(move);
        move_list.add(move);
      } // if
      else if (can_leap(i) &&
               can_move(i, _piece[i], d + LEAP_NORTH, to) &&
               can_capture(i, _board[to]) &&
               !is_blocked(_piece[i], d))
      {
        Move move = Move(i, _piece[i], to, _board[to]);
        do_move(move);
        move.set_value(evaluate());
        undo_move(move);
        move_list.add(move);
      } // if
    } // for
  } // for
  move_list.sort();
  return;
} // Position::generate_moves

void Position::do_move(Move const &move)
{
  _history[_turn] = _key;
  _board[move.from()] = NONE;
  _board[move.to()] = move.piece();
  _piece[move.piece()] = move.to();
  if (move.is_capture())
  {
    _piece[move.enemy()] = CAPTURED;
    _key ^= ZOBRIST[move.enemy()][move.to()];
  } // if
  _key ^= ZOBRIST[move.piece()][move.from()];
  _key ^= ZOBRIST[move.piece()][move.to()];
  _key ^= ZOBRIST_TURN;
  _turn++;
  return;
} // Position::do_move

void Position::undo_move(Move const &move)
{
  _board[move.from()] = move.piece();
  _board[move.to()] = move.enemy();
  _piece[move.piece()] = move.from();
  if (move.is_capture())
  {
    _piece[move.enemy()] = move.to();
    _key ^= ZOBRIST[move.enemy()][move.to()];
  } // if
  _key ^= ZOBRIST[move.piece()][move.from()];
  _key ^= ZOBRIST[move.piece()][move.to()];
  _key ^= ZOBRIST_TURN;
  _turn--;
  return;
} // Position::undo_move

void Position::do_null_move(void)
{
  _history[_turn] = _key;
  _key ^= ZOBRIST_TURN;
  _turn++;
  return;
} // Position::do_null_move

void Position::undo_null_move(void)
{
  _key ^= ZOBRIST_TURN;
  _turn--;
  return;
} // Position::undo_null_move

// *******************************************************************
// Waar is welk stuk op het bord en op welke plek staat welk stuk en
// is een stuk geslagen of niet. (3)
// *******************************************************************
uint32_t Position::where_is(uint32_t const piece) const
{
  assert(piece < static_cast<uint32_t>(PIECES));
  return _piece[piece];
} // Position::where_is

uint32_t Position::what_is_here(uint32_t const location) const
{
  assert(location < static_cast<uint32_t>(BOARD_SIZE - 1));
  return _board[location];
} // Position::what_is_here

bool Position::is_captured(uint32_t const piece) const
{
  assert(piece < static_cast<uint32_t>(PIECES));
  return _piece[piece] == CAPTURED;
} // Position::is_captured

// *******************************************************************
// (Afgeleide) Eigenschappen van de positie op het bord. (7)
// *******************************************************************
bool Position::is_white_turn(void) const
{
  return static_cast<uint32_t>(_turn & 1) == WHITE;
} // Position::is_white_turn

uint64_t Position::key(void) const
{
  return _key;
} // Position::key

int Position::turn(void) const
{
  return _turn;
} // Position::turn

bool Position::is_repeated(void) const
{
  for (int i = _turn - 2; i >= 0; i -= 2)
  {
    if (_history[i] == _key)
    {
      return true;
    } // if
  } // for
  return false;
} // Position::is_repeated

bool Position::is_threefold_repetition(void) const
{
  int count = 1;
  for (int i = _turn - 2; i >= 0; i -= 2)
  {
    if (_history[i] == _key)
    {
      count++;
      if (count >= 3)
      {
        return true;
      } // if
    } // if
  } // for
  return false;
} // Position::is_threefold_repetition

bool Position::is_won(void) const
{
  bool white_alive = false;
  bool black_alive = false;
  for (int i = 0; i < PIECES; i += 2)
  {
    if (_piece[i] != CAPTURED)
    {
      white_alive = true;
    } // if
    if (_piece[i + 1] != CAPTURED)
    {
      black_alive = true;
    } // if
    if (white_alive && black_alive)
    {
      break;
    } // if
  } // for
  return _board[LOCATION_WHITE_DEN] != NONE ||
         _board[LOCATION_BLACK_DEN] != NONE ||
         !white_alive || !black_alive;
} // Position::is_won

bool Position::no_more_moves(void)
{
  Move_list move_list;
  generate_moves(move_list);
  return move_list.size() == 0;
} // Position::no_more_moves

bool Position::last_turn(void) const
{
  return _turn >= MAX_LENGTH;
} // Position::last_turn

// *******************************************************************
// METHOD initial: begin configuratie.
// *******************************************************************
void Position::initial(void)
{
  clear();
  set_piece(WHITE_RAT, G3);
  set_piece(BLACK_RAT, A7);
  set_piece(WHITE_CAT, B2);
  set_piece(BLACK_CAT, F8);
  set_piece(WHITE_WOLF, C3);
  set_piece(BLACK_WOLF, E7);
  set_piece(WHITE_DOG, F2);
  set_piece(BLACK_DOG, B8);
  set_piece(WHITE_PANTHER, E3);
  set_piece(BLACK_PANTHER, C7);
  set_piece(WHITE_TIGER, A1);
  set_piece(BLACK_TIGER, G9);
  set_piece(WHITE_LION, G1);
  set_piece(BLACK_LION, A9);
  set_piece(WHITE_ELEPHANT, A3);
  set_piece(BLACK_ELEPHANT, G7);
  _turn = 0;
  _key = calculate_key();
  return;
} // Position::initial

// *******************************************************************
// METHOD validate_move: kijkt of een zet geldig is in deze
// positie en past eventueel de zet aan. Handig voor door de
// gebruiker ingegeven zetten.
// *******************************************************************
bool Position::validate_move(Move &move)
{
  Move_list move_list;
  generate_moves(move_list);
  if (!is_white_turn())
  {
    move.to_black();
  } // if
  for (int i = 0; i < move_list.size(); i++)
  {
    if (move_list.move(i).piece() == move.piece() &&
        move_list.move(i).to() == move.to())
    {
      move = move_list.move(i);
      return true;
    } // if
  } // for
  return false;
} // Position::validate_move

// *******************************************************************
// Bordpositie kopie\"eren (multi threading).
// *******************************************************************
Position &Position::operator=(Position const &position)
{
  copy(position);
  return *this;
} // Position::operator=

// *******************************************************************
// Grafische weergave van de bordpositie.
// *******************************************************************
void Position::draw_ASCII(std::ostream &stream) const
{
  uint32_t piece = WHITE_RAT;
  stream << "      a b c d e f g          S  Piece     W  B  Remarks"
         << "\n\n  9   ";
  for (int i = 0; i < BOARD_SIZE - 1; i++)
  {
    if (_board[i] == NONE)
    {
      static_cast<void>(serialize_terrain(stream, i));
    } // if
    else
    {
      stream << PIECE_SYMBOL[_board[i]]; // Note: not serialize_piece!
    } // else
    stream << ' ';
    if (i == BOARD_SIZE - 2)
    {
      stream << "  1\n";
    } // if
    else if (i % 7 == 6)
    {
      stream << "  " << 9 - i / 7;
      switch(piece)
      {
        case WHITE_RAT:
          stream << "      1  Rat       R  r  swims and captures "
                 << "elephant";
          piece += 2;
          break;
        case WHITE_CAT:
          stream << "      2  Cat       C  c";
          piece += 2;
          break;
        case WHITE_WOLF:
          stream << "      3  Wolf      W  w";
          piece += 2;
          break;
        case WHITE_DOG:
          stream << "      4  Dog       D  d";
          piece += 2;
          break;
        case WHITE_PANTHER:
          stream << "      5  Panther   P  p";
          piece += 2;
          break;
        case WHITE_TIGER:
          stream << "      6  Tiger     T  t  leaps over water";
          piece += 2;
          break;
        case WHITE_LION:
          stream << "      7  Lion      L  l  leaps over water";
          piece += 2;
          break;
        case WHITE_ELEPHANT:
          stream << "      8  Elephant  E  e";
          piece += 2;
          break;
      } // switch
      stream << "\n  " << 8 - i / 7 << "   ";
    } // if
  } // for
  stream << "\n      a b c d e f g          evaluation = "
         << evaluate() << "\n";
  return;
} // Position::draw_ASCII

// *******************************************************************
// (De-)Serializatie (2)
// *******************************************************************
std::ostream &Position::serialize(std::ostream &stream) const
{
  stream << _turn << '/';
  for (int i = 0; i < PIECES; i++)
  {
    static_cast<void>(serialize_location(stream, _piece[i]));
  } // for
  return stream;
} // Position::serialize

std::istream &Position::unserialize(std::istream &stream)
{
  char separator;
  clear();
  stream >> _turn >> separator;
  for (uint32_t i = 0; i < static_cast<uint32_t>(PIECES); i++)
  {
    set_piece(i, unserialize_location(stream));
  } // for
  _turn &= 1;
  _key = calculate_key();
  return stream;
} // Position::unserialize

// *******************************************************************
// Lees een reeks zetten (vanaf de begin configuratie) in PGN.
// *******************************************************************
std::istream &Position::unserialize_history(std::istream &stream)
{
  char separator;
  int count;
  initial();
  stream >> count >> separator;
  for (int i = 0; i < count; i++)
  {
    Move move;
    stream >> move;
    if (!validate_move(move))
    {
      break;
    } // if
    do_move(move);
  } // for
  return stream;
} // Position::unserialize_history

// *******************************************************************
// METHOD clear: leeg bord.
// *******************************************************************
void Position::clear(void)
{
  int i;
  for (i = 0; i < PIECES; i++)
  {
    _board[i] = NONE;
    _piece[i] = CAPTURED;
  } // for
  for (; i < BOARD_SIZE; i++)
  {
    _board[i] = NONE;
  } // for
  _key = 0x0ull;
  _turn = WHITE;
  return;
} // Position::clear

void Position::copy(Position const &position)
{
  if (&position != this)
  {
    int i;
    for (i = 0; i < PIECES; i++)
    {
      _board[i] = position._board[i];
      _piece[i] = position._piece[i];
      _history[i] = position._history[i];
    } // for
    for (; i < BOARD_SIZE; i++)
    {
      _board[i] = position._board[i];
      _history[i] = position._history[i];
    } // for
    for (; i < position._turn; i++)
    {
      _history[i] = position._history[i];
    } // for
    _key = position._key;
    _turn = position._turn;
  } // if
  return;
} // Position::copy

bool Position::is_blocked(uint32_t const location,
                          uint32_t const direction) const
{
  assert(location < static_cast<uint32_t>(BOARD_SIZE - 1));
  assert(direction <= LEAP_WEST);
  return _board[location + MOVEMENT_OFFSET[direction]] != NONE ||
         _board[location + 2 * MOVEMENT_OFFSET[direction]] != NONE ||
         (is_vertical(direction) ?
           _board[location + 3 * MOVEMENT_OFFSET[direction]] != NONE :
           false);
} // Position::is_blocked

uint64_t Position::calculate_key(void) const
{
  uint64_t key = is_white_turn() ? 0x0ull : ZOBRIST_TURN;
  for (int i = 0; i < PIECES; i++)
  {
    if (_piece[i] != CAPTURED)
    {
      key ^= ZOBRIST[i][_piece[i]];
    } // if
  } // for
  return key;
} // Position::calculate_key

void Position::set_piece(uint32_t const piece, uint32_t location)
{
  assert(piece < static_cast<uint32_t>(PIECES));
  assert(location < static_cast<uint32_t>(BOARD_SIZE));
  return static_cast<void>(_board[_piece[piece] = location] = piece);
} // Position::set_piece

// *******************************************************************
// (De-)Serialisatie met overloaded operatoren (2)
// *******************************************************************
std::ostream &operator<<(std::ostream   &stream,
                         Position const &position)
{
  return position.serialize(stream);
} // operator<<

std::istream &operator>>(std::istream &stream,
                         Position     &position)
{
  return position.unserialize(stream);
} // operator>>

