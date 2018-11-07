// -*- tab-width:4 ; indent-tabs-mode:nil -*-
#ifndef HRE_RUNTIME_H
#define HRE_RUNTIME_H

#include <unistd.h>

/**
\file runtime.h
\brief Process architecture independent RunTime utilities.
*/

typedef struct {
    char* key;
    int val;
} si_map_entry;

/**
\brief Find the value for a given key, or -1 if it does not exist.
 */
extern int linear_search(si_map_entry map[],const char*key);

/**
\brief Find the key name for a given option, or "not found" if it does not exist.
 */
extern char *key_search(si_map_entry map[],const int val);


/**
 * Open the given library, abort on failure.
 */
extern void *RTdlopen (const char *name);

/**
 * Resolve the symbol with respect to the given library.
 * The libname argument is for error messages only.
 */
extern void *RTdlsym (const char *libname, void *handle, const char *symbol);

extern void *RTtrydlsym (void *handle, const char *symbol);

/** \defgroup rt_timer Simple time measuring functions.*/
/*@{*/

/// Opaque type for a timer.
typedef struct timer *rt_timer_t;

/// Create a new timer.
extern rt_timer_t RTcreateTimer();

/// Destroy a timer.
extern void RTdeleteTimer(rt_timer_t timer);

/// Set time used to 0.
extern void RTresetTimer(rt_timer_t timer);

/// Start the stop-watch running.
extern void RTstartTimer(rt_timer_t timer);

/// Reset and Start the stop-watch running.
extern void RTrestartTimer(rt_timer_t timer);

/// Stop the stopwatch running.
extern void RTstopTimer(rt_timer_t timer);

/// Report the time accumulated in this timer.
extern void RTprintTimer(rt_timer_t timer, const char *msg, ...);

/// Return the real time as a float.
extern float RTrealTime(rt_timer_t timer);

/*}@*/

/** \defgroup rt_sysinfo Functions for retrieving system information. */
/*@{*/

/// Get the number of CPUs.
extern int RTnumCPUs();

/// Get the amount of memory.
extern size_t RTmemSize();

/// Get the cache line size.
extern int RTcacheLineSize();

/*}@*/


extern void *RTmalloc(size_t size);

extern void *RTmallocZero(size_t size);

extern void *RTalign(size_t align, size_t size);

extern void *RTalignZero(size_t align, size_t size);

extern void *RTrealloc(void *rt_ptr, size_t size);

extern void RTfree(void *rt_ptr);

#define HRE_NEW(region,sort) ((sort*)RTmallocZero(sizeof(sort)))
#define RT_NEW(obj) HRE_NEW(NULL,obj)

#endif

