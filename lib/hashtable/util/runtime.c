
// -*- tab-width:4 ; indent-tabs-mode:nil -*-
#if defined(__APPLE__)
#include <sys/sysctl.h>
#include <sys/types.h>
#endif

#ifdef __linux__
    #define _GNU_SOURCE 1
    #include <sched.h> // for sched_getaffinity
#endif
#include <dlfcn.h>
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/times.h>
#include <unistd.h>
#include <sys/mman.h>

#include "util/runtime.h"
#include "util/util.h"

#define OLD_DEBUG DEBUG
#undef DEBUG
#define DEBUG 0

int linear_search(si_map_entry map[],const char*key){
    while(map[0].key){
        if(!strcmp(map[0].key,key)) return map[0].val;
        map++;
    }
    return -1;
}

char *key_search(si_map_entry map[],const int val){
    while(map[0].key){
        if(map[0].val == val) return map[0].key;
        map++;
    }
    return "not found";
}

void *
RTdlopen(const char *name)
{
    void *dlHandle;
    char abs_filename[PATH_MAX];
    char *ret_filename = realpath(name, abs_filename);
    if (ret_filename) {
        dlHandle = dlopen(abs_filename, RTLD_LAZY);
        if (dlHandle == NULL) {
            Abort("%s, Library \"%s\" is not reachable", dlerror(), name);
        }
    } else {
        Abort("Library \"%s\" is not found", name);
    }
    return dlHandle;
}

void *
RTdlsym (const char *libname, void *handle, const char *symbol)
{
    void *ret = dlsym (handle, symbol);
    if (ret == NULL) {
        const char *dlerr = dlerror ();
        Abort("dynamically loading from `%s': %s", libname,
               dlerr != NULL ? dlerr : "unknown error");
    }
    return ret;
}

void *
RTtrydlsym(void *handle, const char *symbol)
{
    return dlsym (handle, symbol);
}

#if defined(__APPLE__)

size_t RTmemSize(){
    const char *memsize = getenv("LTSMIN_MEM_SIZE");
    if (memsize) {
        return strtoimax(memsize, NULL, 10);
    } else {
        int mib[4];
        int64_t physical_memory;
        size_t len = sizeof(int64_t);
        mib[0] = CTL_HW;
        mib[1] = HW_MEMSIZE;
        len = sizeof(int64_t);
        sysctl(mib, 2, &physical_memory, &len, NULL, 0);
        return physical_memory;
    }
}

int RTcacheLineSize(){
    int mib[4];
    int line_size;
    size_t len = sizeof(int);
    mib[0] = CTL_HW;
    mib[1] = HW_CACHELINE;
    len = sizeof(int);
    sysctl(mib, 2, &line_size, &len, NULL, 0);
    return line_size;
}

#else

static int mem_size_warned = 0;

size_t RTmemSize(){
    const char *memsize = getenv("LTSMIN_MEM_SIZE");
    if (memsize) {
        return strtoimax(memsize, NULL, 10);
    } else {
        const long res=sysconf(_SC_PHYS_PAGES);
        const long pagesz=sysconf(_SC_PAGESIZE);
        size_t limit = pagesz*((size_t)res);

        /* Now try to determine whether this program runs in a cgroup.
         * If this is the case, we will pick the minimum of the previously
         * computed limit.
         */
        const char *file = "/sys/fs/cgroup/memory/memory.limit_in_bytes";
        FILE *fp = fopen(file, "r");
        if (fp != NULL) {
            size_t cgroup_limit = 0;
            /* If there is no limit then the value scanned for will be larger
             * than SIZE_T_MAX, and thus fscanf will not return 1.
             */
            int ret = fscanf(fp, "%zu", &cgroup_limit);
            if (ret == 1 && cgroup_limit > 0) {
                if (cgroup_limit < limit) {
                    limit = cgroup_limit;
                    if (!mem_size_warned) {
                        Print("Using cgroup limit of %zu bytes", limit);
                    }
                }
            } else if (!mem_size_warned) {
                Error("Unable to get cgroup memory limit "
                        "in file %s: %s",
                        file, errno != 0 ? strerror(errno) : "unknown error");
            }
            fclose(fp);
        } else if (!mem_size_warned) {
            Error("Unable to open cgroup memory limit file %s: %s",
                    file, strerror(errno));
        }

        mem_size_warned = 1;

        return limit;
    }
}

#if defined(__linux__)

int RTcacheLineSize(){
    long res=sysconf(_SC_LEVEL1_DCACHE_LINESIZE);
    return (int)res;
}

#else

int RTcacheLineSize(){
    Abort("generic implementation for RTcacheLineSize needed");
}

#endif

#endif

int RTnumCPUs(){
    const char *numCPUs = getenv("LTSMIN_NUM_CPUS");
    if (numCPUs) {
        return strtoimax(numCPUs, NULL, 10);
    } else {
        int cpu_count = sysconf(_SC_NPROCESSORS_ONLN);
#ifdef __linux__
        cpu_set_t cpus;
        cpu_set_t* cpus_p = &cpus;
        size_t cpus_size = sizeof(cpu_set_t);
        int configured_cpus = sysconf(_SC_NPROCESSORS_CONF);
        if (configured_cpus >= CPU_SETSIZE) {
            cpus_p = CPU_ALLOC(configured_cpus);
            if (cpus_p) {
                cpus_size = CPU_ALLOC_SIZE(configured_cpus);
                CPU_ZERO_S(cpus_size, cpus_p);
            } else {
                Error("CPU_ALLOC failed: %s",
                        errno != 0 ? strerror(errno) : "unknown error");
                return cpu_count;
            }
        }
        if (!sched_getaffinity(0, cpus_size, cpus_p)) {
            if (cpus_p != &cpus) cpu_count = CPU_COUNT_S(cpus_size, cpus_p);
            else cpu_count = CPU_COUNT(cpus_p);
        } else {
            Error("Unable to get CPU set affinity: %s",
                    errno != 0 ? strerror(errno) : "unknown error");
        }
        if (cpus_p != &cpus) CPU_FREE(cpus_p);
#endif
        return cpu_count;
    }
}

struct timer {
    clock_t real_time;
    struct tms times;
    int running;
};

rt_timer_t RTcreateTimer(){
    rt_timer_t timer;
    timer=(rt_timer_t)RTmalloc(sizeof(struct timer));
    if (timer){
        timer->real_time=0;
        timer->times.tms_utime=0;
        timer->times.tms_stime=0;
        timer->times.tms_cutime=0;
        timer->times.tms_cstime=0;
        timer->running=0;
    }
    return timer;
}

void RTdeleteTimer(rt_timer_t timer){
    RTfree(timer);
}

void RTresetTimer(rt_timer_t timer){
    if (timer->running) {
        timer->real_time=times(&(timer->times));
    } else {
        timer->real_time=0;
        timer->times.tms_utime=0;
        timer->times.tms_stime=0;
        timer->times.tms_cutime=0;
        timer->times.tms_cstime=0;
    }
}

void RTstartTimer(rt_timer_t timer){
    if (!(timer->running)) {
        struct tms tmp;
        clock_t real_time;
        real_time=times(&tmp);
        timer->real_time-=real_time;
        timer->times.tms_utime-=tmp.tms_utime;
        timer->times.tms_stime-=tmp.tms_stime;
        timer->times.tms_cutime-=tmp.tms_cutime;
        timer->times.tms_cstime-=tmp.tms_cstime;
        timer->running=1;
    }
}

void RTrestartTimer(rt_timer_t timer){
    RTresetTimer (timer);
    RTstartTimer (timer);
}

void RTstopTimer(rt_timer_t timer){
    if (timer->running) {
        struct tms tmp;
        clock_t real_time;
        real_time=times(&tmp);
        timer->real_time+=real_time;
        timer->times.tms_utime+=tmp.tms_utime;
        timer->times.tms_stime+=tmp.tms_stime;
        timer->times.tms_cutime+=tmp.tms_cutime;
        timer->times.tms_cstime+=tmp.tms_cstime;
        timer->running=0;
    }
}

void RTprintTimer(rt_timer_t timer,const char *msg, ...){
    clock_t tick=sysconf(_SC_CLK_TCK);
    float tm_real, tm_user, tm_sys;
    if (timer->running) {
        struct tms tmp;
        clock_t real_time;
        real_time=times(&tmp);
        tm_real=((float)(real_time+timer->real_time))/((float)tick);
        tm_user=((float)(tmp.tms_utime+timer->times.tms_utime))/((float)tick);
        tm_sys=((float)(tmp.tms_stime+timer->times.tms_stime))/((float)tick);
    } else {
        tm_real=((float)(timer->real_time))/((float)tick);
        tm_user=((float)(timer->times.tms_utime))/((float)tick);
        tm_sys=((float)(timer->times.tms_stime))/((float)tick);
    }

    va_list argptr;
    va_start(argptr, msg);
    char buf[255];
    vsnprintf(buf, 255, msg, argptr);
    va_end(argptr);
    Print("%s %5.3f real %5.3f user %5.3f sys",buf,tm_real,tm_user,tm_sys);
}

float RTrealTime(rt_timer_t timer){
    clock_t tick=sysconf(_SC_CLK_TCK);
    if (timer->running) {
        struct tms tmp;
        clock_t real_time;
        real_time=times(&tmp);
        return ((float)(real_time+timer->real_time))/((float)tick);
    } else {
        return ((float)(timer->real_time))/((float)tick);
    }
}


void *
RTmalloc(size_t size)
{
    if(size==0) return NULL;
    void *tmp=malloc(size);
    if (tmp==NULL) Abort("out of memory trying to get %zu",size);
    Debug("allocated %zu from system", size);
    return tmp;
}

void *
RTmallocZero(size_t size)
{
    if(size==0) return NULL;
    void *tmp=calloc((size + CACHE_LINE_SIZE - 1) >> CACHE_LINE, CACHE_LINE_SIZE);
    if (tmp==NULL) Abort("out of memory trying to get %zu",size);
    Debug("allocated %zu in anonymous pages from system", size);
    return tmp;
}

void *
RTalign(size_t align, size_t size)
{
    void *ret = NULL;
    errno = posix_memalign(&ret, align, size);
    if (errno) {
    switch (errno) {
        case ENOMEM:
            Abort("out of memory on allocating %zu bytes aligned at %zu",
                  size, align);
        case EINVAL:
            Abort("invalid alignment %zu", align);
        default:
            Abort("unknown error allocating %zu bytes aligned at %zu",
                  size, align);
    }}
    Assert (NULL != ret, "Alloc failed");
    Debug("allocated %zu aligned at %zu from system", size, align);
    return ret;
}

#define MAX_ALIGN_MEMSET (1024*1024)
#define MAX_ALIGN_ZEROS (1024)
static size_t next_calloc = 0;
static void *calloc_table[MAX_ALIGN_ZEROS][3];

void *
RTalignZero(size_t align, size_t size)
{
    if (0 == align) Abort("Zero alignment in RTalignZero");
    if (0 == size) return NULL;
    if (size < MAX_ALIGN_MEMSET) {
        // for small sizes do memset
        void *mem = RTalign(align, size);
        memset (mem, 0 , size);
        Debug("allocated %zu in anonymous pages aligned at %zu from system", size, align);
        return mem;
    }
    // for large sizes use calloc and do manual alignment
    if ((size / align)*align != size) // make size multiple of align
        size = ((size + align) / align)*align;
//    void *p = calloc((size / align + 1), align);
    void *p = mmap (NULL,size,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANON,-1,0);
    // MAP_PRIVATE && WRITE, because we still want the attached NUMA policy to be followed
    if (p == MAP_FAILED)
        Abort ("mmap failed for size %zu MB", size>>20);
    size_t pp = (size_t)p;
    void *old = p;
    if ((pp / align) * align != pp) // manual alignment only if needed
        p = (void*)((pp / align + 1) * align);
    // store self-aligned allocs in calloc_table
    size_t next = __sync_fetch_and_add (&next_calloc, 1);
    calloc_table[next][0] = p;
    calloc_table[next][1] = old;
    calloc_table[next][2] = (void*)size;
    Debug("allocated %zu in (LARGE) anonymous pages aligned at %zu from system", size, align);
    return p;
}

void *
RTrealloc(void *rt_ptr, size_t size)
{
    if (size==0) { // macosx realloc(??.0) does not return NULL!
        free(rt_ptr);
        return NULL;
    }
    Debug("reallocating %p to size %zu from system", rt_ptr, size);
    void *tmp=realloc(rt_ptr,size);
    if (tmp==NULL) Abort("out of memory trying to resize to %zu",size);
    return tmp;
}

void
RTfree(void *rt_ptr)
{
    Debug("freeing %p from system", rt_ptr);
    free(rt_ptr);
}

#undef DEBUG
#define DEBUG OLD_DEBUG

