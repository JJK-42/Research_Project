// *******************************************************************
//   (C) Copyright 2013 Leiden Institute of Advanced Computer Science
//   Universiteit Leiden
//   All Rights Reserved
// *******************************************************************
// Kunstmatige Intelligentie --- Jungle
// *******************************************************************
// FILE INFORMATION:
//   File:     types.h
//   Author:   Jonathan K. Vis
//   Revision: 0.01a
//   Date:     2013/01/18
// *******************************************************************
// DESCRIPTION:
//   Types voor tenminste 32-bits en 64-bits woorden en een aantal
//   globale constanten.
// *******************************************************************

#ifndef __types_h__
#define __types_h__

typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

static uint32_t const WHITE = 0;
static uint32_t const BLACK = 1;

static uint32_t const NONE = 16;
static uint32_t const CAPTURED = 63;

static int const BOARD_SIZE = 64;
static int const PIECES = 16;

static int const MAX_MOVES = 32;
static int const MAX_LENGTH = 512;

#endif // __types_h__

