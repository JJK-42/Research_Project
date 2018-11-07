#include <stdio.h>
#include <cstring>
#include <set>
#include <pthread.h>

#include <unistd.h>
#include <stdlib.h>


extern "C" {
#include <util/MurmurHash3.h>
#include "util/hashtable.h"

static int
strcomp (const char *s1, const char *s2, void *ctx)
{
    return strcmp (s1, s2);
    (void) ctx;
}

static uint32_t
strhash (const char *str, void *ctx)
{
    return MurmurHash32(str, strlen(str), 0);
    (void) ctx;
}

static char *
strclone (char *str, void *ctx)
{
    return strdup(str);
    (void) ctx;
}

static void
strfree (void *str)
{
    free (str);
}

}

using namespace std;

hashtable_t *months;

void *run_thread(void* arg)
{
    pair<const char*, int>* p = (pair<const char*, int>*)arg;
	
	map_val_t val = (map_val_t) p->second;
    map_key_t out;
  
  if(strcomp(p->first, "february", NULL) == 0)
  {
		int ret = (int)ht_cas(months, (map_key_t)p->first, (map_val_t)27, val, &out, NULL);
		printf("%i =? %i\n", ret, p->second);
	}
	else
	{
		ht_cas_empty (months, (map_key_t) p->first, val, &out, NULL);
  }
  // (For the ctx parameter of ht_cas_empty,
  //  you could pass thread local data for use in strcopy, strcomp, etc)

	int x = (int)ht_get(months, (map_key_t)p->first, NULL);
	printf("Set months[%s] to %i (is now %i)\n", p->first, p->second, x);
	pthread_exit(NULL);
}

int main()
{
    datatype_t dtypes = (datatype_t) { (cmp_fun_t) strcomp,
                                       (hash_fun_t) strhash,
                                       (clone_fun_t) strclone,
                                       (free_fun_t) strfree };

    months = ht_alloc(&dtypes, 10); // intial size 2^10


  const int N = 13;
  int n = N;
  pthread_t threads[n];
  pair<const char*, int> p[n];
  p[--n] = make_pair("january", 31);
  p[--n] = make_pair("february", 27);
  p[--n] = make_pair("february", 28);
  p[--n] = make_pair("march", 31);
  p[--n] = make_pair("april", 30);
  p[--n] = make_pair("may", 31);
  p[--n] = make_pair("june", 30);
  p[--n] = make_pair("july", 31);
  p[--n] = make_pair("august", 31);
  p[--n] = make_pair("september", 30);
  p[--n] = make_pair("october", 31);
  p[--n] = make_pair("november", 30);
  p[--n] = make_pair("december", 31);

  for(int i = 0; i < N; i++)
        pthread_create(&(threads[i]), NULL, run_thread, &(p[i]));

  for(pthread_t thread : threads)
        pthread_join(thread, NULL);

  for(size_t i = 0; i < N; i++) {
      int x = (int)ht_get(months, (map_key_t)p[i].first, NULL);
      printf("%s (%i): %i ... %i\n", p[i].first, p[i].second, x, p[i].second == x);
  }
}


