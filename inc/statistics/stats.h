#ifndef STATS_H
#define STATS_H

#include <list>
#include <utility>

class Statistics
{
	public:
		/**
		 * Constructor.
		 * Initialises private variables.
		 */
		Statistics();
		
		/**
		 * Resets all variables to default values.
		 * Empties lists.
		 */
		void reset();
		
		/**
		 * Records current time in milliseconds as the timer's start.
		 * Returns this start time as well.
		 */
		int start_timer();
		/**
		 * Calls get_time_elapsed and records the result in a list.
		 * Returns the recorded elapsed time.
		 */
		int record_time();
		/**
		 * Calculates and returns how much time has passed since the timer
		 * has been started.
		 */
		int get_time_elapsed();
		
		/**
		 * Adds the leaf to the list of traversed nodes and increments
		 * nodes_evaluated.
		 */
		void record_leaf(const int leaf);
		/**
		 * Adds the node to the list of traversed nodes and increments
		 * nodes_expanded.
		 */
		void record_expand(const int node);
		
		/**
		 * Increments hash table entries counter
		 */
		void inc_ht_entry();
		
		/**
		 * Increments successful lookups counter
		 */
		void inc_succ_lookups();
		
		/**
		 * Merges this with the given stats by summing both objects'
		 * nodes_evaluated, nodes_expanded, ht_entries, ht_succ_lookups
		 * and by merging the times lists. (Doesn't merge traversal)
		 */
		void merge(Statistics* stats);
		
		/**
		 * Prints hash table entries counter and succesful lookups counter
		 */
		void print_ht();
		
		/**
		 * Prints the list of traversed nodes. Adds a line break after every
		 * leaf node.
		 */
		void print_traversal();
		/**
		 * Prints the general statistics:
		 * - nodes_evaluated
		 * - nodes_expanded
		 * - a list of all recorded times in the timer
		 */
		void print_stats();
	
	private:
		int nodes_evaluated;
		int nodes_expanded;
		int timer_start;
		
		int ht_entries;
		int ht_succ_lookups;
		
		std::list<int> times;
		std::list<std::pair<int, bool>> traversal;
		
};

#endif
