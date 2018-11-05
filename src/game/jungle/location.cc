// *******************************************************************
//   (C) Copyright 2013 Leiden Institute of Advanced Computer Science
//   Universiteit Leiden
//   All Rights Reserved
// *******************************************************************
// Kunstmatige Intelligentie --- Jungle
// *******************************************************************
// FILE INFORMATION:
//   File:     location.cc (Depends on: location.h)
//   Author:   Jonathan K. Vis
//   Revision: 0.01a
//   Date:     2013/01/18
// *******************************************************************
// DESCRIPTION:
//   Implementatie van de (de-)serialisatiefuncties voor locaties.
// *******************************************************************

#include "location.h"

std::ostream &serialize_location(std::ostream  &stream,
                                 uint32_t const location)
{
  assert(location <= BOARD_SIZE - 1);
  return stream << LOCATION_LABEL[location];
} // serialize_location

uint32_t unserialize_location(std::istream &stream)
{
  char buffer[2] = {stream.get(), stream.get()};
  return 7 * (9 - static_cast<uint32_t>(buffer[1] - '0')) + 
                  static_cast<uint32_t>(buffer[0] - 'a');
} // unserialize_location

