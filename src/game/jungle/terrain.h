// *******************************************************************
//   (C) Copyright 2013 Leiden Institute of Advanced Computer Science
//   Universiteit Leiden
//   All Rights Reserved
// *******************************************************************
// Kunstmatige Intelligentie --- Jungle
// *******************************************************************
// FILE INFORMATION:
//   File:     terrain.h (Implementation file: terrain.cc)
//   Author:   Jonathan K. Vis
//   Revision: 0.01a
//   Date:     2013/01/18
// *******************************************************************
// DESCRIPTION:
//   Beschrijving van de layout van het bord met betrekking to het
//   soort locatie (normaal, water, etc.). Het bord ziet er als volgt
//   uit (zonder stukken):
//   
//     . . ^ # ^ . .
//     . . . ^ . . .
//     . . . . . . .
//     . ~ ~ . ~ ~ .
//     . ~ ~ . ~ ~ .
//     . ~ ~ . ~ ~ .
//     . . . . . . .
//     . . . ^ . . .
//     . . ^ # ^ . .
//
//   waarbij:
//     ~  water
//     ^  val (wit of zwart)
//     #  `den' (wit of zwart)
// *******************************************************************

#ifndef __terrain_h__
#define __terrain_h__

#include "types.h"

#include <cassert>
#include <iostream>

// *******************************************************************
// Eigenschappen van een locatie
// *******************************************************************
bool is_water(uint32_t const location);
bool is_white_trap(uint32_t const location);
bool is_black_trap(uint32_t const location);

// *******************************************************************
// Serializatiefunctie
// *******************************************************************
std::ostream &serialize_terrain(std::ostream  &stream,
                                uint32_t const location);

static uint32_t const FLOOR      = 0;
static uint32_t const WATER      = 1;
static uint32_t const WHITE_TRAP = 2;
static uint32_t const BLACK_TRAP = 3;
static uint32_t const WHITE_DEN  = 4;
static uint32_t const BLACK_DEN  = 5;

static char const TERRAIN_SYMBOL[BLACK_DEN + 1] =
{
  '.',
  '~',
  '^', '^',
  '#', '#'
}; // TERRAIN_SYMBOL

static uint32_t const TERRAIN[BOARD_SIZE] =
{
  FLOOR, FLOOR, BLACK_TRAP, BLACK_DEN,  BLACK_TRAP, FLOOR, FLOOR,
  FLOOR, FLOOR, FLOOR,      BLACK_TRAP, FLOOR,      FLOOR, FLOOR,
  FLOOR, FLOOR, FLOOR,      FLOOR,      FLOOR,      FLOOR, FLOOR,
  FLOOR, WATER, WATER,      FLOOR,      WATER,      WATER, FLOOR,
  FLOOR, WATER, WATER,      FLOOR,      WATER,      WATER, FLOOR,
  FLOOR, WATER, WATER,      FLOOR,      WATER,      WATER, FLOOR,
  FLOOR, FLOOR, FLOOR,      FLOOR,      FLOOR,      FLOOR, FLOOR,
  FLOOR, FLOOR, FLOOR,      WHITE_TRAP, FLOOR,      FLOOR, FLOOR,
  FLOOR, FLOOR, WHITE_TRAP, WHITE_DEN,  WHITE_TRAP, FLOOR, FLOOR,
  FLOOR
}; // TERRAIN

#endif // __terrain_h__

