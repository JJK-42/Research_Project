// *******************************************************************
//   (C) Copyright 2013 Leiden Institute of Advanced Computer Science
//   Universiteit Leiden
//   All Rights Reserved
// *******************************************************************
// Kunstmatige Intelligentie --- Jungle
// *******************************************************************
// FILE INFORMATION:
//   File:     move.h (Implementation file: move.cc)
//   Author:   Jonathan K. Vis
//   Revision: 0.01a
//   Date:     2013/01/18
// *******************************************************************
// DESCRIPTION:
//   Klasse om een zet in te bewaren. Hierin wordt opgeslagen welk
//   stuk van welke locatie naar een andere locatie gaat en of het
//   daarbij eventueel een stuk van de tegenstander slaat. Ook kan een
//   waardering aan de zet worden toegekent (om zetten te kunnen
//   ordenen). Het inlezen en afdrukken van een zet gebeurt altijd
//   vanuit het standpunt van wit (het witte symbool voor een stuk
//   wordt gebruikt, vergelijk: schaken).
// *******************************************************************

#ifndef __move_h__
#define __move_h__

#include "location.h"
#include "pieces.h"
#include "types.h"

#include <cassert>
#include <iostream>

class Move
{
  public:
    // ***************************************************************
    // CONSTRUCTORS
    // ***************************************************************
    Move(void);
    Move(uint32_t piece,
         uint32_t from,
         uint32_t to,
         uint32_t enemy);

    // ***************************************************************
    // Toegang to de private members
    // ***************************************************************
    uint32_t piece(void) const;
    uint32_t from(void) const;
    uint32_t to(void) const;
    uint32_t enemy(void) const;
    int      value(void) const;

    // ***************************************************************
    // Afgeleide eigenschappen: is_valid zegt alleen iets over het
    // formaat waarin de zet staat (handig bij inlezen), niet of de
    // zet op dit bord geldig is (zie position.h).
    // ***************************************************************
    bool is_capture(void) const;
    bool is_valid(void) const;

    // ***************************************************************
    // MEMBER set_value: kent een waardering toe aan een zet zodat
    // zetten geordend kunnen worden. (niet gebruiken?)
    // ***************************************************************
    void set_value(int const value);
    // ***************************************************************
    // MEMBER to_black: converteert een ingelezen zet (altijd met een
    // wit stuk) naar het zwarte stuk. (niet gebruiken?)
    // ***************************************************************
    void to_black(void);

    // ***************************************************************
    // (De-)Serialisatie: gebruik liever de overloaded operatoren!
    // ***************************************************************
    std::ostream &serialize(std::ostream &stream) const;
    std::istream &unserialize(std::istream &stream);

  private:
    uint32_t _piece;
    uint32_t _from;
    uint32_t _to;
    uint32_t _enemy;
    int      _value;
 
}; // Move

// *******************************************************************
// (De-)Serialisatie met overloaded operatoren (handig)
// *******************************************************************
std::ostream &operator<<(std::ostream &stream, Move const &move);
std::istream &operator>>(std::istream &stream, Move &move);

#endif // __move_h__

