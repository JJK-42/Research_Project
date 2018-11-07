

cmake_minimum_required(VERSION 2.7)

include_directories(..)


add_library(util STATIC
        dfs-stack.c dfs-stack.h
        dynamic-array.h dynamic-array.c
        fast_hash.h fast_hash.c
        is-balloc.c is-balloc.h
        list.h list.c
        MurmurHash3.h MurmurHash3.c
        runtime.h runtime.c
        sort_r.h
        util.h)

add_library(cht STATIC
            hashtable.h hashtable.c
            atomics.h)

target_link_libraries(cht util) 