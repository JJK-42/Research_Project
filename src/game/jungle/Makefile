## *******************************************************************
##   (C) Copyright 2013 Leiden Institute of Advanced Computer Science
##   Universiteit Leiden
##   All Rights Reserved
## *******************************************************************
## Kunstmatige Intelligentie --- Jungle
## *******************************************************************
## FILE INFORMATION:
##   File:     Makefile
##   Author:   Jonathan K. Vis
##   Revision: 0.01a
##   Date:     2013/01/18
## *******************************************************************
## DESCRIPTION:
##   Gebruik het commando `make' om het project te compileren. Of
##   wanneer headers zijn aangepast bebruik dan `make rebuild'. Indien
##   er additionele codebestanden zijn, dienen deze worden toegevoegd
##   aan de variabele `SEARCHSOURCES'. Pas de variabele `EXECUTABLE'
##   aan. Ook kunnen de compileervlaggen worden aangepast bijvoorbeeld
##   aan het gebruik van een debugger of het uitzetten van
##   optimalisatie.
## *******************************************************************

CC=g++
CFLAGS=-c -Wall -Wextra -O3
LDFLAGS=-Wall -Wextra -O3
HEADERS=location.h \
        move.h \
        move_list.h \
        movement.h \
        pieces.h \
        position.h \
        search.h \
        terrain.h \
        types.h \
        zobrist.h
GAMESOURCES=location.cc \
            move.cc \
            move_list.cc \
            movement.cc \
            pieces.cc \
            position.cc \
            terrain.cc
SEARCHSOURCES=evaluation.cc \
              main.cc \
              search.cc
SOURCES=$(GAMESOURCES) $(SEARCHSOURCES)
GAMEOBJECTS=$(GAMESOURCES:.cc=.o)
SEARCHOBJECTS=$(SEARCHSOURCES:.cc=.o)
OBJECTS=$(GAMEOBJECTS) $(SEARCHOBJECTS)
EXECUTABLE=jungle
SCRIPTS=contest.sh

.PHONY: all clean clean_all rebuild rebuild_all tarball

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cc.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(EXECUTABLE) $(SEARCHOBJECTS)

clean_all: clean
	rm -f $(GAMEOBJECTS)

rebuild: clean all

rebuild_all: clean_all all

tarball:
	tar -cvzf jungle.tgz $(HEADERS) $(SOURCES) $(SCRIPTS) Makefile

