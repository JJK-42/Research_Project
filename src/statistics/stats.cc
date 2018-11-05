/*
 * Class made to keep track of algorithm statistics.
 * Statistics currently implemented:
 * - Number of nodes evaluated in the game tree
 * - Number of nodes expanded in the game tree
 * - Duration (in milliseconds)
 */
#include <sys/time.h>
#include <stdio.h>

#include "statistics/stats.h"

using namespace std;

Statistics::Statistics()
{
	reset();
}

void Statistics::reset()
{
	nodes_evaluated = 0;
	nodes_expanded = 0;
	timer_start = -1;
	times.clear();
	traversal.clear();
	ht_entries = 0;
	ht_succ_lookups = 0;
}

int Statistics::start_timer()
{
	struct timeval tp;
	gettimeofday(&tp, NULL);
	return timer_start = tp.tv_sec * 1000 + tp.tv_usec / 1000;
}

int Statistics::record_time()
{
	int time_elapsed = get_time_elapsed();
	times.push_back(time_elapsed);
	return time_elapsed;
}

int Statistics::get_time_elapsed()
{
	struct timeval tp;
	gettimeofday(&tp, NULL);
	return tp.tv_sec * 1000 + tp.tv_usec / 1000 - timer_start;
}

void Statistics::record_leaf(const int leaf)
{
	traversal.push_back(make_pair(leaf, true));
	nodes_evaluated++;
}

void Statistics::record_expand(const int node)
{
	traversal.push_back(make_pair(node, false));
	nodes_expanded++;
}

void Statistics::inc_ht_entry()
{
	ht_entries++;
}

void Statistics::inc_succ_lookups()
{
	ht_succ_lookups++;
}

void Statistics::print_ht()
{
	printf("Hash table has %i new entries.\n", ht_entries);
	printf("Existing entries were retrieved %i times.\n", ht_succ_lookups);
}

void Statistics::print_traversal()
{
	printf("Node traversal:\n");
	for(auto entry : traversal)
	{
		printf("%i ", entry.first);
		if(entry.second)
			printf("\n");
	}
	printf("\n");
}

void Statistics::print_stats()
{
	printf("--- Algorithm statistics ---\n");
	printf("| Nodes evaluated: %7i |\n", nodes_evaluated);
	printf("| Nodes expanded:  %7i |\n", nodes_expanded);
	printf("| Recorded times:          |\n");
	for(auto entry : times)
		printf("| - %22i |\n", entry);
	printf("----------------------------\n");
}
