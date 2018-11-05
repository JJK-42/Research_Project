// *******************************************************************
//   (C) Copyright 2013 Leiden Institute of Advanced Computer Science
//   Universiteit Leiden
//   All Rights Reserved
// *******************************************************************
// Kunstmatige Intelligentie --- Jungle
// *******************************************************************
// FILE INFORMATION:
//   File:     move.cc (Depends on: move.h)
//   Author:   Jonathan K. Vis
//   Revision: 0.01a
//   Date:     2013/01/18
// *******************************************************************
// DESCRIPTION:
//   Implementatie van de lijst van zetten klasse.
// *******************************************************************

#include "move_list.h"

void quicksort(Move      move[],
               int const left,
               int const right); // forward declaration

// *******************************************************************
// CONSTRUCTOR: geen zetten in de lijst
// *******************************************************************
Move_list::Move_list(void)
{
  _size = 0;
} // Move_list::Move_list

// *******************************************************************
// Toegang to de private members (2)
// *******************************************************************
Move Move_list::move(int const index) const
{
  assert(index < _size);
  return _move[index];
} // Move_list::move

int Move_list::size(void) const
{
  return _size;
} // Move_list::size

// *******************************************************************
// MEMBER add: een zet toevoegen aan de lijst.
// *******************************************************************
void Move_list::add(Move const &move)
{
  assert(_size < MAX_MOVES);
  return static_cast<void>(_move[_size++] = move);
} // Move_list::add

// *******************************************************************
// MEMBER sort: de lijst aflopend sorteren.
// *******************************************************************
void Move_list::sort(void)
{
  return quicksort(_move, 0, _size - 1);
} // Move_list::sort

// *******************************************************************
// Serialisatie
// *******************************************************************
std::ostream &Move_list::serialize(std::ostream &stream) const
{
  stream << _size << '/';
  for (int i = 0; i < _size; i++)
  {
    stream << _move[i];
  } // for
  return stream;
} // Move_list::serialize

// *******************************************************************
// Serialisatie met overloaded operator
// *******************************************************************
std::ostream &operator<<(std::ostream    &stream,
                         Move_list const &move_list)
{
  return move_list.serialize(stream);
} // operator<<

// *******************************************************************
// FUNCTION quicksort: aflopend sorteren.
// *******************************************************************
void quicksort(Move      move[],
               int const left,
               int const right)
{
  int i = left;
  int j = right;
  int const pivot = move[(left + right) >> 1].value();
  while (i <= j)
  {
    while (move[i].value() < pivot)
    {
      i++;
    } // while
    while (move[j].value() > pivot)
    {
      j--;
    } // while
    if (i <= j)
    {
      Move const temp = move[i];
      move[i] = move[j];
      move[j] = temp;
      i++;
      j--;
    } // if
  } // while
  if (left < j)
  {
    quicksort(move, left, j);
  } // if
  if (i < right)
  {
    quicksort(move, i, right);
  } // if
  return;
} // quicksort

