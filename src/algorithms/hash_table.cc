/**
 * 
 */

#include <stdio.h>

#include "algorithms/hash_table.h"

using namespace std;

static int game_comp(const Game* g1, const Game* g2, void* ctx)
{
	return g1 == g2;//verify
	(void) ctx;
}

static uint32_t game_hash(const Game* game, void* ctx)
{
	return game->get_hash();
	(void) ctx;
}

static Game* game_clone(Game* game, void* ctx)
{
	return game->clone();
	(void) ctx;
}

static void game_free(void* game)
{
	printf("game_free unimplemented\n");
	//TODO
}

static datatype_t dtypes = (datatype_t)
{
	(cmp_fun_t) game_comp,
	(hash_fun_t) game_hash,
	(clone_fun_t) game_clone,
	(free_fun_t) game_free
};

Hashtable::Hashtable()
{
	alpha_map = ht_alloc(&dtypes, 10);
	beta_map = ht_alloc(&dtypes, 10);
	stats = new Statistics();
}

Hashtable::Hashtable(Statistics* statistics)
{
	alpha_map = ht_alloc(&dtypes, 10);
	beta_map = ht_alloc(&dtypes, 10);
	stats = statistics;
}

int Hashtable::get_alpha(Game* key)
{
	map_val_t val = ht_get(alpha_map, (map_key_t)key, NULL);
	
	if(val == DOES_NOT_EXIST)
	{
		stats->inc_ht_entry();
		map_key_t out;
		ht_cas_empty(alpha_map, (map_key_t)key, (map_val_t)ALPHA, &out, NULL);
		return ALPHA;
	}
	else
	{
		stats->inc_succ_lookups();
		return (int)val;
	}
}

int Hashtable::get_beta(Game* key)
{
	map_val_t val = ht_get(beta_map, (map_key_t)key, NULL);
	
	if(val == DOES_NOT_EXIST)
	{
		stats->inc_ht_entry();
		map_key_t out;
		ht_cas_empty(beta_map, (map_key_t)key, (map_val_t)BETA, &out, NULL);
		return BETA;
	}
	else
	{
		stats->inc_succ_lookups();
		return (int)val;
	}
}

bool Hashtable::increase_alpha(Game* key, int val, int comp_val)
{
	map_key_t out;
	map_val_t old = ht_get(alpha_map, (map_key_t)key, NULL);
	
	//printf("Putting a %i at %i\n", val, key->get_hash());
	
	if(val >= comp_val && val > (int)old)
		ht_cas(alpha_map, (map_key_t)key, old,
	       (map_val_t)val, &out, NULL);
	else
		return false;
	
	return true;
}

bool Hashtable::increase_beta(Game* key, int val, int comp_val)
{
	map_key_t out;
	map_val_t old = ht_get(beta_map, (map_key_t)key, NULL);
	
	//printf("Putting b %i at %i\n", val, key->get_hash());
	
	if(val <= comp_val && val < (int)old)
		ht_cas(beta_map, (map_key_t)key, old,
	       (map_val_t)val, &out, NULL);
	else
		return false;
	
	return true;
}

bool Hashtable::swap_alpha(Game* key, int val)
{
	map_key_t out;
	map_val_t old = ht_get(alpha_map, (map_key_t)key, NULL);
	
	//printf("Putting a %i at %i\n", val, key->get_hash());
	
	if((int)old < val)
		ht_cas(alpha_map, (map_key_t)key, old,
		       (map_val_t)val, &out, NULL);
	else
		return false;
	
	return true;
}

bool Hashtable::swap_beta(Game* key, int val)
{
	map_key_t out;
	map_val_t old = ht_get(beta_map, (map_key_t)key, NULL);
	
	//printf("Putting b %i at %i\n", val, key->get_hash());
	
	if((int)old > val)
		ht_cas(beta_map, (map_key_t)key, old,
		       (map_val_t)val, &out, NULL);
	else
		return false;
	
	return true;
}
