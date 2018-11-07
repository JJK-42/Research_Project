#include <stdio.h>
#include <pthread.h>

#include <unistd.h>
#include <stdlib.h>
#include <string.h>

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


hashtable_t *months;

static const char *name[] = {
   "February",
   "January", "February", "March",
   "April", "May", "June",
   "July", "August", "September",
   "October", "November", "December"
};

static int days[] = {
   27,
   31,
   28,
   31,
   30,
   31,
   30,
   31,
   31,
   30,
   31,
   30,
   31
};

void *run_thread(void* arg)
{
    size_t i = (size_t)arg;

	map_val_t val = (map_val_t) days[i];
	map_key_t out;
	ht_cas_empty (months, (map_key_t)name[i], val, &out, NULL);
    // (For the ctx parameter of ht_cas_empty,
	//  you could pass thread local data for use in strcopy, strcomp, etc)

	int x = (int)ht_get(months, (map_key_t)name[i], NULL);
	printf("Set months[%s] to %i (is now %i)\n", name[i], days[i], x);
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

    printf("Starting threads\n");
    for (size_t i = 0; i < N; i++)
        pthread_create(&(threads[i]), NULL, run_thread, (void *) i);

    printf("Waiting for threads\n");
    for (size_t i = 0; i < N; i++)
        pthread_join(threads[i], NULL);

    for (size_t i = 0; i < N; i++) {
        int x = (int)ht_get(months, (map_key_t)name[i], NULL);
        printf("%s (%i): %i ... %i\n", name[i], days[i], x, days[i] == x);
    }
}


