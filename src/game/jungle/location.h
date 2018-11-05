// *******************************************************************
//   (C) Copyright 2013 Leiden Institute of Advanced Computer Science
//   Universiteit Leiden
//   All Rights Reserved
// *******************************************************************
// Kunstmatige Intelligentie --- Jungle
// *******************************************************************
// FILE INFORMATION:
//   File:     location.h (Implementation file: location.cc)
//   Author:   Jonathan K. Vis
//   Revision: 0.01a
//   Date:     2013/01/18
// *******************************************************************
// DESCRIPTION:
//   We gebruiken een algebra\"ische notatie (AN), zoals ook bij
//   schaken:
//     http://en.wikipedia.org/wiki/Algebraic_notation_%28chess%29
//   om locaties op het bord te beschrijven. Deze beschrijvingen
//   kunnen vervolgens worden gebruikt voor het beschrijven van
//   zetten.
//   Iedere locatie op het bord wordt beschreven met een letter (a--g)
//   voor de kolom en een cijfer (1--9) voor de rij gezien vanuit de
//   positie van wit. De locaties op het bord zien er als volgt uit:
//
//     a9 b9 c9 d9 e9 f9 g9
//     a8 b8 c8 d8 e8 f8 g8
//     a7 b7 c7 d7 e7 f7 g7
//     a6 b6 c6 d6 e6 f6 g6
//     a5 b5 c5 d5 e5 f5 g5
//     a4 b4 c4 d4 e4 f4 g4
//     a3 b3 c3 d3 e3 f3 g3
//     a2 b2 c2 d2 e2 f2 g2
//     a1 b1 c1 d1 e1 f1 g1
//
// *******************************************************************

#ifndef __location_h__
#define __location_h__

#include "types.h"

#include <cassert>
#include <iostream>

// *******************************************************************
// (De-)Serialisatiefuncties: gebruik om in te lezen en af te drukken
// *******************************************************************
std::ostream &serialize_location(std::ostream  &stream,
                                 uint32_t const location);
uint32_t unserialize_location(std::istream &stream);

static uint32_t const A9 =  0;
static uint32_t const B9 =  1;
static uint32_t const C9 =  2;
static uint32_t const D9 =  3;
static uint32_t const E9 =  4;
static uint32_t const F9 =  5;
static uint32_t const G9 =  6;
static uint32_t const A8 =  7;
static uint32_t const B8 =  8;
static uint32_t const C8 =  9;
static uint32_t const D8 = 10;
static uint32_t const E8 = 11;
static uint32_t const F8 = 12;
static uint32_t const G8 = 13;
static uint32_t const A7 = 14;
static uint32_t const B7 = 15;
static uint32_t const C7 = 16;
static uint32_t const D7 = 17;
static uint32_t const E7 = 18;
static uint32_t const F7 = 19;
static uint32_t const G7 = 20;
static uint32_t const A6 = 21;
static uint32_t const B6 = 22;
static uint32_t const C6 = 23;
static uint32_t const D6 = 24;
static uint32_t const E6 = 25;
static uint32_t const F6 = 26;
static uint32_t const G6 = 27;
static uint32_t const A5 = 28;
static uint32_t const B5 = 29;
static uint32_t const C5 = 30;
static uint32_t const D5 = 31;
static uint32_t const E5 = 32;
static uint32_t const F5 = 33;
static uint32_t const G5 = 34;
static uint32_t const A4 = 35;
static uint32_t const B4 = 36;
static uint32_t const C4 = 37;
static uint32_t const D4 = 38;
static uint32_t const E4 = 39;
static uint32_t const F4 = 40;
static uint32_t const G4 = 41;
static uint32_t const A3 = 42;
static uint32_t const B3 = 43;
static uint32_t const C3 = 44;
static uint32_t const D3 = 45;
static uint32_t const E3 = 46;
static uint32_t const F3 = 47;
static uint32_t const G3 = 48;
static uint32_t const A2 = 49;
static uint32_t const B2 = 50;
static uint32_t const C2 = 51;
static uint32_t const D2 = 52;
static uint32_t const E2 = 53;
static uint32_t const F2 = 54;
static uint32_t const G2 = 55;
static uint32_t const A1 = 56;
static uint32_t const B1 = 57;
static uint32_t const C1 = 58;
static uint32_t const D1 = 59;
static uint32_t const E1 = 60;
static uint32_t const F1 = 61;
static uint32_t const G1 = 62;

static uint32_t const LOCATION_WHITE_DEN = D1;
static uint32_t const LOCATION_BLACK_DEN = D9;

static char const* const LOCATION_LABEL[BOARD_SIZE] =
{
  "a9", "b9", "c9", "d9", "e9", "f9", "g9",
  "a8", "b8", "c8", "d8", "e8", "f8", "g8",
  "a7", "b7", "c7", "d7", "e7", "f7", "g7",
  "a6", "b6", "c6", "d6", "e6", "f6", "g6",
  "a5", "b5", "c5", "d5", "e5", "f5", "g5",
  "a4", "b4", "c4", "d4", "e4", "f4", "g4",
  "a3", "b3", "c3", "d3", "e3", "f3", "g3",
  "a2", "b2", "c2", "d2", "e2", "f2", "g2",
  "a1", "b1", "c1", "d1", "e1", "f1", "g1",
  "h1" // (captured; should *NOT* be used)
}; // LOCATION_LABEL

#endif // __location_h__

