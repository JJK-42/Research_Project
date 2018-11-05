// *******************************************************************
//   (C) Copyright 2013 Leiden Institute of Advanced Computer Science
//   Universiteit Leiden
//   All Rights Reserved
// *******************************************************************
// Kunstmatige Intelligentie --- Jungle
// *******************************************************************
// FILE INFORMATION:
//   File:     move_list.h (Implementation file: move_list.cc)
//   Author:   Jonathan K. Vis
//   Revision: 0.01a
//   Date:     2013/01/18
// *******************************************************************
// DESCRIPTION:
//   Klasse om een lijst van mogelijke zetten (voor een bepaalde
//   positie op het bord) in op te slaan en te ordenen naar waarde.
// *******************************************************************
#ifndef __move_list_h__
#define __move_list_h__

#include "move.h"
#include "types.h"

#include <cassert>
#include <iostream>

class Move_list
{
  public:
    // ***************************************************************
    // CONSTRUCTOR
    // ***************************************************************
    Move_list(void);

    // ***************************************************************
    // Toegang to de private members
    // ***************************************************************
    Move move(int const index) const;
    int  size(void) const;

    // ***************************************************************
    // MEMBER add: een zet toevoegen aan de lijst.
    // ***************************************************************
    void add(Move const &move);
    // ***************************************************************
    // MEMBER sort: de lijst aflopend sorteren op de waarde van de
    // zetten.
    // ***************************************************************
    void sort(void);

    // ***************************************************************
    // Serialisatie: gebruik liever de overloaded operator!
    // ***************************************************************
    std::ostream &serialize(std::ostream &stream) const;

  private:
    Move _move[MAX_MOVES];
    int  _size;

}; // Move_list

// *******************************************************************
// Serialisatie met overloaded operator (handig)
// *******************************************************************
std::ostream &operator<<(std::ostream    &stream,
                         Move_list const &move_list);

#endif // __move_list_h__

