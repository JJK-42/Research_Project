// *******************************************************************
//   (C) Copyright 2013 Leiden Institute of Advanced Computer Science
//   Universiteit Leiden
//   All Rights Reserved
// *******************************************************************
// Kunstmatige Intelligentie --- Jungle
// *******************************************************************
// FILE INFORMATION:
//   File:     evaluation.cc (Depends on: position.h)
//   Author:   /You/
//   Revision: 0.01a
//   Date:     2013/01/18
// *******************************************************************
// DESCRIPTION:
//   Implementatie van de evaluatiefunctie. Deze functie moet een
//   geheel getal opleveren op basis van de positie op het bord.
//   De evaluatie wordt vanuit het standpunt van de witte speler
//   bekeken: een groter (positief) getal geeft een voordeel voor wit.
//   Gebruikelijk is dat deze functie symmetrisch is rond 0. Dat wil
//   zeggen, indien een positie wordt gewaardeerd voor wit met x,
//   wordt deze positie voor zwart met -x gewaardeerd.
//   Een aantal factoren kunnen in deze evaluatiefunctie worden
//   opgenomen. Waarbij direct valt te denken aan de relatieve sterkte
//   van de zich nog op het bord bevindende stukken en hun locaties
//   op het bord. Bijvoorbeeld een stuk dichtbij de `den' van de
//   tegenstander lijkt gunstig.
// *******************************************************************

#include "position.h"

// *******************************************************************
// MEMBER evaluate: Nu slechts een gelijkspel.
// *******************************************************************
int Position::evaluate(void) const
{
  int value = 0;
  return is_white_turn() ? value : -value;
} // Position::evaluate

