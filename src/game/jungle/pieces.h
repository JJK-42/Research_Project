// *******************************************************************
//   (C) Copyright 2013 Leiden Institute of Advanced Computer Science
//   Universiteit Leiden
//   All Rights Reserved
// *******************************************************************
// Kunstmatige Intelligentie --- Jungle
// *******************************************************************
// FILE INFORMATION:
//   File:     pieces.h (Implementation file: pieces.cc)
//   Author:   Jonathan K. Vis
//   Revision: 0.01a
//   Date:     2013/01/18
// *******************************************************************
// DESCRIPTION:
//   We gebruiken een algebra\"ische notatie (AN), zoals ook bij
//   schaken:
//     http://en.wikipedia.org/wiki/Algebraic_notation_%28chess%29
//   om de stukken op het bord te beschrijven. De stukken worden
//   beschreven als volgt:
//   
//   wit  zwart  sterkte  omschrijving
//   =================================================================
//   R    r      0        rat; kan de olifant (sterkte 7) slaan en kan
//                        zich in het water bewegen.
//   C    c      1        kat
//   W    w      2        wolf
//   D    d      3        hond
//   P    p      4        luipaard
//   T    t      5        tijger; kan over het water springen
//   L    l      6        leeuw; kan over het water springen
//   E    e      7        olifant
//   
//   Let op: bij het in lezen en afdrukken wordt altijd het symbool
//   voor het witte stuk gebruikt (vergelijk: schaken).
// *******************************************************************

#ifndef __pieces_h__
#define __pieces_h__

#include "terrain.h"
#include "types.h"

#include <cassert>
#include <iostream>

// *******************************************************************
// Eigenschappen van de stukken
// *******************************************************************
bool is_elephant(uint32_t const piece);
bool is_white(uint32_t const piece);
bool is_black(uint32_t const piece);
bool can_leap(uint32_t const piece);
bool is_enemy(uint32_t const piece_a, uint32_t const piece_b);
int strength(uint32_t const piece);
bool can_capture(uint32_t const piece_a, uint32_t const piece_b);
bool is_trapped(uint32_t const piece, uint32_t const location);

// *******************************************************************
// (De-)Serialisatie functies: gebruik om in te lezen en af te drukken
// *******************************************************************
std::ostream &serialize_piece(std::ostream  &stream,
                              uint32_t const piece);
uint32_t unserialize_piece(std::istream &stream);

static uint32_t const WHITE_RAT      =  0;
static uint32_t const BLACK_RAT      =  1;
static uint32_t const WHITE_CAT      =  2;
static uint32_t const BLACK_CAT      =  3;
static uint32_t const WHITE_WOLF     =  4;
static uint32_t const BLACK_WOLF     =  5;
static uint32_t const WHITE_DOG      =  6;
static uint32_t const BLACK_DOG      =  7;
static uint32_t const WHITE_PANTHER  =  8;
static uint32_t const BLACK_PANTHER  =  9;
static uint32_t const WHITE_TIGER    = 10;
static uint32_t const BLACK_TIGER    = 11;
static uint32_t const WHITE_LION     = 12;
static uint32_t const BLACK_LION     = 13;
static uint32_t const WHITE_ELEPHANT = 14;
static uint32_t const BLACK_ELEPHANT = 15;

static char const PIECE_SYMBOL[PIECES] =
{
  'R', 'r',
  'C', 'c',
  'W', 'w',
  'D', 'd',
  'P', 'p',
  'T', 't',
  'L', 'l',
  'E', 'e'
}; // PIECE_SYMBOL

#endif // __pieces_h__

