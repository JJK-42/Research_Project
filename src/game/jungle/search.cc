// *******************************************************************
//   (C) Copyright 2013 Leiden Institute of Advanced Computer Science
//   Universiteit Leiden
//   All Rights Reserved
// *******************************************************************
// Kunstmatige Intelligentie --- Jungle
// *******************************************************************
// FILE INFORMATION:
//   File:     search.cc (Depends on: search.h)
//   Author:   /You/
//   Revision: 0.02a
//   Date:     2013/01/28
// *******************************************************************
// DESCRIPTION:
//   Implementatie van de zoekalgoritmen. Nu wordt er niet erg zinnig
//   gezocht.
//   2013/01/28 ADDED: variabelen voor het bijhouden van het aantal
//                     bezochte knopen.
// *******************************************************************

#include "search.h"

uint64_t node_count = 0;
uint64_t leaf_count = 0;

int negamax(Position &position,
            int const depth,
            Move     &move)
{
  if (depth == 0)
  {
    return position.evaluate();
  } // if
  return -negamax(position, depth - 1, move);
} // negamax

int alphabeta(Position &position,
              int const depth,
              int const alpha,
              int const beta,
              Move     &move)
{
  if (depth == 0)
  {
    return position.evaluate();
  } // if
  return -alphabeta(position, depth - 1, -beta, -alpha, move);
} // alphabeta

