CC=g++
CFLAGS=-c -Wall -Iinc -std=c++0x -O0 -g

HEADERS=inc/algorithms/hash_table.h\
				inc/algorithms/algorithm.h\
				inc/algorithms/ab_rollouts.h\
				inc/algorithms/ab_rol_par.h\
				inc/algorithms/ab_simple.h\
				inc/game/game.h\
				inc/statistics/stats.h

CLHTLIBS=-Llib/ -lclht -lssmem
SPARSEHASHLIBS=-Ilib/sparsehash
HASHTABLELIB=-Llib/ -Ilib/hashtable -lcht -lutil -ldl
HASHTABLEHEADERS=lib/hashtable/util/hashtable.h

CILK=-fcilkplus -lcilkrts
PTHREAD=-lpthread

HASHHS=$(HASHTABLEHEADERS)
HASHLIBS=$(HASHTABLELIB)
CPAR=$(PTHREAD)

all: ab_rol_par play_hex_game clean

clean:
	rm *.o

#ab_simple: ab_simple.o algorithm.o simple_game.o stats.o
#	$(CC) ab_simple.o algorithm.o simple_game.o stats.o -o ab_simple

#ab_rol_simple: ab_rollouts.o algorithm.o simple_game.o stats.o
#	$(CC) ab_rollouts.o algorithm.o simple_game.o stats.o -o ab_rol_simple

#ab_rol_par_simple: ab_rollouts_parallel.o algorithm.o simple_game.o stats.o
#	$(CC) ab_rollouts_parallel.o algorithm.o simple_game.o stats.o -o ab_rol_par_simple

ab_hex: run_ab_hex.o ab_simple.o algorithm.o hash_table.o hex_game.o stats.o HexState.o
	$(CC) run_ab_hex.o ab_simple.o algorithm.o hash_table.o hex_game.o stats.o HexState.o -o ab_hex $(HASHLIBS)

ab_rol_hex: run_rol_hex.o ab_rollouts.o algorithm.o hash_table.o hex_game.o stats.o HexState.o
	$(CC) run_rol_hex.o ab_rollouts.o algorithm.o hash_table.o hex_game.o stats.o HexState.o -o ab_rol_hex $(HASHLIBS)

ab_rol_par: run_rol_par.o ab_rol_par.o ab_rollouts.o algorithm.o hash_table.o hex_game.o stats.o HexState.o
	$(CC) run_rol_par.o ab_rol_par.o ab_rollouts.o algorithm.o hash_table.o hex_game.o stats.o HexState.o -o ab_rol_par $(HASHLIBS) $(CPAR)

ab_par_hex: run_ab_par.o ab_par.o algorithm.o hash_table.o hex_game.o stats.o
	$(CC) $(CFLAGS) run_ab_par.o ab_par.o algorithm.o hash_table.o hex_game.o stats.o -o ab_par_hex $(HASHLIBS)

#ab_rol_par_simple: ab_rollouts_parallel.o algorithm.o simple_game.o stats.o
#	$(CC) ab_rollouts_parallel.o algorithm.o simple_game.o stats.o -o ab_rol_par_simple $(CFLAGS)

play_hex_game: play_game.o ab_simple.o ab_rollouts.o algorithm.o hex_game.o HexState.o hash_table.o stats.o
	$(CC) play_game.o ab_simple.o ab_rollouts.o algorithm.o hex_game.o HexState.o hash_table.o stats.o -o play_hex_game $(HASHLIBS)

play_simple_game: play_game.o ab_simple.o ab_rollouts.o algorithm.o simple_game.o stats.o
	$(CC) $(CFLAGS) play_game.o ab_simple.o ab_rollouts.o algorithm.o simple_game.o stats.o -o play_simple_game $(HASHLIBS)

algorithm.o: src/algorithms/algorithm.cc $(HEADERS) 
	$(CC) $(CFLAGS) src/algorithms/algorithm.cc $(HASHLIBS) $(CFLAGS)

ab_simple.o: src/algorithms/ab_simple.cc $(HEADERS) 
	$(CC) $(CFLAGS) src/algorithms/ab_simple.cc $(HASHLIBS)

ab_rol_par.o: src/algorithms/ab_rol_par.cc $(HEADERS) 
	$(CC) $(CFLAGS) src/algorithms/ab_rol_par.cc $(HASHLIBS)

ab_rollouts.o: src/algorithms/ab_rollouts.cc $(HEADERS)
	$(CC) $(CFLAGS) src/algorithms/ab_rollouts.cc $(HASHLIBS)

#ab_rollouts_parallel.o: src/algorithms/ab_rollouts_parallel.cc inc/game/game.h inc/statistics/stats.h
#	$(CC) $(CFLAGS) src/algorithms/ab_rollouts_parallel.cc

hash_table.o: src/algorithms/hash_table.cc $(HEADERS) $(HASHHS)
	$(CC) $(CFLAGS) src/algorithms/hash_table.cc $(HASHLIBS)

simple_game.o: src/game/simple_game.cc $(HEADERS)
	$(CC) $(CFLAGS) src/game/simple_game.cc $(HASHLIBS)

hex_game.o: src/game/hex_game.cc inc/game/game.h inc/game/hex/HexState.h
	$(CC) $(CFLAGS) src/game/hex_game.cc $(HASHLIBS)

HexState.o: src/game/hex/HexState.cpp inc/game/hex/HexState.h inc/game/hex/Utilities.h
	$(CC) $(CFLAGS) src/game/hex/HexState.cpp $(HASHLIBS)

stats.o: src/statistics/stats.cc inc/statistics/stats.h
	$(CC) $(CFLAGS) src/statistics/stats.cc $(HASHLIBS)

play_game.o: src/main/play_game.cc inc/play_game.h inc/game/game.h inc/algorithms/algorithm.h inc/algorithms/ab_simple.h inc/algorithms/ab_rollouts.h
	$(CC) $(CFLAGS) src/main/play_game.cc $(HASHLIBS)

run_ab_hex.o: src/main/run_ab_hex.cc inc/game/game.h inc/algorithms/ab_simple.h
	$(CC) $(CFLAGS) src/main/run_ab_hex.cc $(HASHLIBS)

run_rol_hex.o: src/main/run_rol_hex.cc inc/game/game.h inc/algorithms/ab_rollouts.h
	$(CC) $(CFLAGS) src/main/run_rol_hex.cc $(HASHLIBS)
	
run_rol_par.o: src/main/run_rol_par.cc inc/game/game.h inc/algorithms/ab_rol_par.h
	$(CC) $(CFLAGS) src/main/run_rol_par.cc $(HASHLIBS)
