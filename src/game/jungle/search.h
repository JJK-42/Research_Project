// *******************************************************************
//   (C) Copyright 2013 Leiden Institute of Advanced Computer Science
//   Universiteit Leiden
//   All Rights Reserved
// *******************************************************************
// Kunstmatige Intelligentie --- Jungle
// *******************************************************************
// FILE INFORMATION:
//   File:     search.h (Implementation file: search.cc)
//   Author:   /You/
//   Revision: 0.02a
//   Date:     2013/01/28
// *******************************************************************
// DESCRIPTION:
//   Definitie van de zoekalgoritmen: negamax en alpha-beta. Deze
//   moeten in het bestand `search.cc' worden ge\"implementeerd.
//   2013/01/28 ADDED: variabelen voor het bijhouden van het aantal
//                     bezochte knopen.
// *******************************************************************

#ifndef __search_h__
#define __search_h__

#include "move.h"
#include "move_list.h"
#include "position.h"

#include <cassert>
#include <cstdlib>

// Gebruik deze variabelen om het aantal bezochte knopen (en bladeren)
// bij te houden.
extern uint64_t node_count;
extern uint64_t leaf_count;

int negamax(Position &position,
            int const depth,
            Move     &move);

int alphabeta(Position &position,
              int const depth,
              int const alpha,
              int const beta,
              Move     &move);

#endif // __search_h__

