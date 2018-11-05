// *******************************************************************
//   (C) Copyright 2013 Leiden Institute of Advanced Computer Science
//   Universiteit Leiden
//   All Rights Reserved
// *******************************************************************
// Kunstmatige Intelligentie --- Jungle
// *******************************************************************
// FILE INFORMATION:
//   File:     position.h (Implementation files: position.cc
//                                               evaluation.cc)
//   Author:   Jonathan K. Vis
//   Revision: 0.02a
//   Date:     2013/02/05
// *******************************************************************
// DESCRIPTION:
//   Houdt de spelconfiguratie bij in een object van de klasse
//   Position.
//   2013/02/05 ADDED: een drietal functies met toegang tot de
//                     private members mbt de plaats van de stukken op
//                     het bord
// *******************************************************************

#ifndef __position_h__
#define __position_h__

#include "location.h"
#include "move.h"
#include "move_list.h"
#include "movement.h"
#include "pieces.h"
#include "terrain.h"
#include "types.h"
#include "zobrist.h"

#include <cassert>
#include <iostream>

class Position
{
  public:
    // ***************************************************************
    // CONSTRUCTORS
    // ***************************************************************
    Position(void);
    Position(Position const &position);

    // ***************************************************************
    // Genereren, doen en ongedaan maken van zetten.
    // ***************************************************************
    void generate_moves(Move_list &move_list);
    void do_move(Move const &move);
    void undo_move(Move const &move);

    // ***************************************************************
    // Doet en maakt een null-zet ongedaan (null move pruning).
    // ***************************************************************
    void do_null_move(void);
    void undo_null_move(void);
    
    // ***************************************************************
    // METHOD evaluate: evaluatiefunctie moet worden ge\"implementeerd
    // in het bestand `evaluation.cc'.
    // ***************************************************************
    int evaluate(void) const;

    // ***************************************************************
    // Waar is welk stuk op het bord en op welke plek staat welk stuk
    // en is een stuk geslagen of niet.
    // ***************************************************************
    uint32_t where_is(uint32_t const piece) const;
    uint32_t what_is_here(uint32_t const location) const;
    bool     is_captured(uint32_t const piece) const;

    // ***************************************************************
    // (Afgeleide) Eigenschappen van de positie op het bord.
    // ***************************************************************
    bool     is_white_turn(void) const;
    uint64_t key(void) const;
    int      turn(void) const;
    bool     is_repeated(void) const;
    bool     is_threefold_repetition(void) const;
    bool     is_won(void) const;
    bool     no_more_moves(void);
    bool     last_turn(void) const;

    // ***************************************************************
    // METHOD initial: begin configuratie.
    // ***************************************************************
    void     initial(void);

    // ***************************************************************
    // METHOD validate_move: kijkt of een zet geldig is in deze
    // positie en past eventueel de zet aan. Handig voor door de
    // gebruiker ingegeven zetten.
    // ***************************************************************
    bool     validate_move(Move &move);

    // ***************************************************************
    // Bordpositie kopie\"eren (multi threading).
    // ***************************************************************
    Position &operator=(Position const &position);

    // ***************************************************************
    // Grafische weergave van de bordpositie.
    // ***************************************************************
    void         draw_ASCII(std::ostream &stream) const;

    // ***************************************************************
    // (De-)Serializatie: gebruik liever de overloaded operatoren!
    // ***************************************************************
    std::ostream &serialize(std::ostream &stream) const;
    std::istream &unserialize(std::istream &stream);
    // ***************************************************************
    // Lees een reeks zetten (vanaf de begin configuratie) in
    // Portable Game Notation (PGN):
    //   http://en.wikipedia.org/wiki/Portable_Game_Notation
    // ***************************************************************
    std::istream &unserialize_history(std::istream &stream);

  private:
    void clear(void);
    void copy(Position const &position);

    // ***************************************************************
    // METHOD is_blocked: is een sprong mogelijk?
    // ***************************************************************
    bool is_blocked(uint32_t const location,
                    uint32_t const direction) const;

    uint64_t calculate_key(void) const;
    void     set_piece(uint32_t const piece, uint32_t const location);

    // ***************************************************************
    // MEMBER _board[]: wat staat er op welke locatie op het bord.
    // ***************************************************************
    uint32_t _board[BOARD_SIZE];
    // ***************************************************************
    // MEMBER _piece[]: waar staat ieder stuk op het bord.
    // ***************************************************************
    uint32_t _piece[PIECES];

    uint64_t _key;
    int      _turn;
    uint64_t _history[MAX_LENGTH];

}; // Position

// *******************************************************************
// (De-)Serialisatie met overloaded operatoren (handig)
// *******************************************************************
std::ostream &operator<<(std::ostream   &stream,
                         Position const &position);
std::istream &operator>>(std::istream &stream,
                         Position     &position);

#endif // __position_h__

