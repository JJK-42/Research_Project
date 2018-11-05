// *******************************************************************
//   (C) Copyright 2013 Leiden Institute of Advanced Computer Science
//   Universiteit Leiden
//   All Rights Reserved
// *******************************************************************
// Kunstmatige Intelligentie --- Jungle
// *******************************************************************
// FILE INFORMATION:
//   File:     pieces.cc (Depends on: pieces.h)
//   Author:   Jonathan K. Vis
//   Revision: 0.01a
//   Date:     2013/01/18
// *******************************************************************
// DESCRIPTION:
//   Implementatie van de functies voor de stukken.
// *******************************************************************

#include "pieces.h"

// *******************************************************************
// Eigenschappen van de stukken (8)
// *******************************************************************
bool is_elephant(uint32_t const piece)
{
  assert(piece < static_cast<uint32_t>(PIECES));
  return piece >= WHITE_ELEPHANT;
} // is_elephant

bool is_white(uint32_t const piece)
{
  assert(piece < static_cast<uint32_t>(PIECES));
  return (piece & 1) == WHITE;
} // is_white

bool is_black(uint32_t const piece)
{
  assert(piece < static_cast<uint32_t>(PIECES));
  return (piece & 1) == BLACK;
} // is_black

bool can_leap(uint32_t const piece)
{
  assert(piece < static_cast<uint32_t>(PIECES));
  return piece >= WHITE_TIGER && piece <= BLACK_LION;
} // can_leap

bool is_enemy(uint32_t const piece_a, uint32_t const piece_b)
{
  assert(piece_a < static_cast<uint32_t>(PIECES));
  assert(piece_b < static_cast<uint32_t>(PIECES));
  return (is_white(piece_a) && is_black(piece_b)) ||
         (is_black(piece_a) && is_white(piece_b));
} // is_enemy

int strength(uint32_t const piece)
{
  assert(piece < static_cast<uint32_t>(PIECES));
  return static_cast<int>(piece >> 1);
} // strength

bool can_capture(uint32_t const piece_a, uint32_t const piece_b)
{
  assert(piece_a < static_cast<uint32_t>(PIECES));
  assert(piece_b < static_cast<uint32_t>(PIECES) || piece_b == NONE);
  return piece_b == NONE || (is_enemy(piece_a, piece_b) &&
         strength(piece_a) >= strength(piece_b));
} // can_capture

bool is_trapped(uint32_t const piece, uint32_t const location)
{
  assert(piece < static_cast<uint32_t>(PIECES));
  assert(location < static_cast<uint32_t>(BOARD_SIZE - 1));
  return (is_white(piece) && is_black_trap(location)) ||
         (is_black(piece) && is_white_trap(location));
} // is_trapped

// *******************************************************************
// (De-)Serialisatie functies (2)
// *******************************************************************
std::ostream &serialize_piece(std::ostream  &stream,
                              uint32_t const piece)
{
  assert(piece < static_cast<uint32_t>(PIECES));
  return stream << PIECE_SYMBOL[piece];
} // serialize_piece

uint32_t unserialize_piece(std::istream &stream)
{
  switch (stream.get())
  {
    case 'R':
      return WHITE_RAT;
    case 'C':
      return WHITE_CAT;
    case 'W':
      return WHITE_WOLF;
    case 'D':
      return WHITE_DOG;
    case 'P':
      return WHITE_PANTHER;
    case 'T':
      return WHITE_TIGER;
    case 'L':
      return WHITE_LION;
    case 'E':
      return WHITE_ELEPHANT;
  } // switch
  return NONE;
} // unserialize_piece

