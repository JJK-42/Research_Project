// *******************************************************************
//   (C) Copyright 2013 Leiden Institute of Advanced Computer Science
//   Universiteit Leiden
//   All Rights Reserved
// *******************************************************************
// Kunstmatige Intelligentie --- Jungle
// *******************************************************************
// FILE INFORMATION:
//   File:     terrain.cc (Depends on: terrain.h)
//   Author:   Jonathan K. Vis
//   Revision: 0.01a
//   Date:     2013/01/18
// *******************************************************************
// DESCRIPTION:
//   Implementatie van de functies voor het soort locatie.
// *******************************************************************

#include "terrain.h"

// *******************************************************************
// Eigenschappen van een locatie (3)
// *******************************************************************
bool is_water(uint32_t const location)
{
  assert(location < static_cast<uint32_t>(BOARD_SIZE - 1));
  return TERRAIN[location] == WATER;
} // is_water

bool is_white_trap(uint32_t const location)
{
  assert(location < static_cast<uint32_t>(BOARD_SIZE - 1));
  return TERRAIN[location] == WHITE_TRAP;
} // is_white_trap

bool is_black_trap(uint32_t const location)
{
  assert(location < static_cast<uint32_t>(BOARD_SIZE - 1));
  return TERRAIN[location] == BLACK_TRAP;
} // is_black_trap

// *******************************************************************
// Serializatiefunctie
// *******************************************************************
std::ostream &serialize_terrain(std::ostream  &stream,
                                uint32_t const location)
{
  assert(location < static_cast<uint32_t>(BOARD_SIZE - 1));
  return stream << TERRAIN_SYMBOL[TERRAIN[location]];
} // serialize_terrain

