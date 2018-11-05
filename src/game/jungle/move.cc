// *******************************************************************
//   (C) Copyright 2013 Leiden Institute of Advanced Computer Science
//   Universiteit Leiden
//   All Rights Reserved
// *******************************************************************
// Kunstmatige Intelligentie --- Jungle
// *******************************************************************
// FILE INFORMATION:
//   File:     move.cc (Depends on: move.h)
//   Author:   Jonathan K. Vis
//   Revision: 0.01a
//   Date:     2013/01/18
// *******************************************************************
// DESCRIPTION:
//   Implementatie van de zet klasse.
// *******************************************************************

#include "move.h"

// *******************************************************************
// CONSTRUCTORS (2)
// *******************************************************************
Move::Move(void)
{
  // intentionally empty
} // Move::Move

Move::Move(uint32_t piece,
           uint32_t from,
           uint32_t to,
           uint32_t enemy)
{
  assert(piece <= static_cast<uint32_t>(PIECES));
  assert(from <= static_cast<uint32_t>(BOARD_SIZE));
  assert(to <= static_cast<uint32_t>(BOARD_SIZE));
  assert(enemy <= static_cast<uint32_t>(PIECES));
  _piece = piece;
  _from = from;
  _to = to;
  _enemy = enemy;
} // Move::Move

// *******************************************************************
// Toegang to de private members (5)
// *******************************************************************
uint32_t Move::piece(void) const
{
  return _piece;
} // Move::piece

uint32_t Move::from(void) const
{
  return _from;
} // Move::from

uint32_t Move::to(void) const
{
  return _to;
} // Move::to

uint32_t Move::enemy(void) const
{
  return _enemy;
} // Move::enemy

int Move::value(void) const
{
  return _value;
} // Move::value

// *******************************************************************
// Afgeleide eigenschappen (2)
// *******************************************************************
bool Move::is_capture(void) const
{
  return _enemy != NONE;
} // Move::is_capture

bool Move::is_valid(void) const
{
  return _piece < NONE && _to < static_cast<uint32_t>(BOARD_SIZE - 1);
} // Move::is_valid

// *******************************************************************
// Zet aanpassen: waardering en conversie naar zwart (2)
// *******************************************************************
void Move::set_value(int const value)
{
  return static_cast<void>(_value = value);
} // Move::set_value

void Move::to_black(void)
{
  assert(_piece < static_cast<uint32_t>(PIECES - 1));
  assert(is_white(_piece));
  return static_cast<void>(_piece++);
} // Move::to_black

// *******************************************************************
// (De-)Serialisatie (2)
// *******************************************************************
std::ostream &Move::serialize(std::ostream &stream) const
{
  assert(_piece < static_cast<uint32_t>(PIECES));
  assert(_to < static_cast<uint32_t>(BOARD_SIZE - 1));
  static_cast<void>(serialize_piece(stream, (_piece >> 1) << 1));
  if (is_capture())
  {
    stream << 'x';
  } // if
  return serialize_location(stream, _to);
} // Move::serialize

std::istream &Move::unserialize(std::istream &stream)
{
  char buffer = stream.peek();
  if (buffer == ' ' || buffer == '\n')
  {
    buffer = stream.get(); // ignore whitespace
  } // if
  buffer = stream.peek();
  if (buffer >= '0' && buffer <= '9')
  {
    int move_number;
    stream >> move_number;
    buffer = stream.get(); // ignore '.'
  } // if
  _piece = unserialize_piece(stream);
  _from = CAPTURED;
  buffer = stream.peek();
  if (buffer == 'x')
  {
    buffer = stream.get(); // ignore 'x'
  } // if
  _to = unserialize_location(stream);
  _enemy = NONE;
  return stream;
} // Move::unserialize

// *******************************************************************
// (De-)Serialisatie met overloaded operatoren (2)
// *******************************************************************
std::ostream &operator<<(std::ostream &stream, Move const &move)
{
  return move.serialize(stream);
} // operator<<

std::istream &operator>>(std::istream &stream, Move &move)
{
  return move.unserialize(stream);
} // operator>>

