cmake_minimum_required(VERSION 2.7)

set(CMAKE_MACOSX_RPATH 1)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#set(CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS_DEBUG}     -fPIC -Wall")
#set(CMAKE_CPP_FLAGS_DEBUG   "${CMAKE_CPP_FLAGS_DEBUG}   -fPIC -Wall -DDEBUG=1 -g -O0")
#set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   -fPIC -Wall -DDEBUG=1 -g -O0")
#set(CMAKE_LD_FLAGS_DEBUG    "${CMAKE_LD_FLAGS_DEBUG}    -g")
#set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE}   -fPIC -Wall -DHAVE_SYLVAN")
#set(CMAKE_CPP_FLAGS_RELEASE "${CMAKE_CPP_FLAGS_RELEASE} -fPIC -Wall")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fPIC -Wall")
#set(CMAKE_LD_FLAGS_RELEASE  "${CMAKE_LD_FLAGS_RELEASE}")

add_definitions("-fPIC -Wall")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions("-DDEBUG=1 -g -O0")
endif()


include_directories(.)

ADD_CUSTOM_TARGET (distclean)

ADD_CUSTOM_COMMAND(
    DEPENDS clean
    COMMENT "distribution clean"
    COMMAND rm
    ARGS    -rf *.dSYM */*.dSYM Makefile */Makefile */CMakeFiles CMakeFiles CMakeCache.txt Makefile */Makefile  */CMakeCache.txt cmake_install.cmake */cmake_install.cmake
    TARGET  distclean
)

add_subdirectory(util)

add_executable(months
                months.c
)

add_executable(months++
                months.cc
)

target_link_libraries(months cht util pthread dl) 
target_link_libraries(months++ cht util pthread dl)
