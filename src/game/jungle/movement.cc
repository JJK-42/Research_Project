// *******************************************************************
//   (C) Copyright 2013 Leiden Institute of Advanced Computer Science
//   Universiteit Leiden
//   All Rights Reserved
// *******************************************************************
// Kunstmatige Intelligentie --- Jungle
// *******************************************************************
// FILE INFORMATION:
//   File:     movement.cc (Depends on: movement.h)
//   Author:   Jonathan K. Vis
//   Revision: 0.01a
//   Date:     2013/01/18
// *******************************************************************
// DESCRIPTION:
//   Implementatie van de bewegingsfuncties.
// *******************************************************************

#include "movement.h"

// *******************************************************************
// FUNCTION is_vertical: gebruikt voor verticale sprongen
// *******************************************************************
bool is_vertical(uint32_t const direction)
{
  assert(direction <= LEAP_WEST);
  return (direction & 1) == 0;
} // is_vertical

// *******************************************************************
// FUNCTION can_move: gegeven een stuk, zijn locatie en een richting
// wordt er bepaald of dit in beginsel mogelijk is (sprongen zijn
// in dit opzicht bijzonder) en de parameter `to' wordt op de geldige
// locatie index gezet.
// *******************************************************************
bool can_move(uint32_t const piece,
              uint32_t const from,
              uint32_t const direction,
              uint32_t      &to)
{
  assert(piece < static_cast<uint32_t>(PIECES));
  assert(from < static_cast<uint32_t>(BOARD_SIZE));
  assert(direction <= LEAP_WEST);
  to = from + MOVEMENT_OFFSET[direction];
  return (MOVEMENT[piece][from] & MOVEMENT_MASK[direction]) ==
         MOVEMENT_MASK[direction];
} // can_move

