cmake_minimum_required(VERSION 3.13)
project(OS_Ex3)

set(CMAKE_CXX_STANDARD 11)

add_executable(OS_Ex3 Barrier.h Barrier.cpp testListHooks.cpp ThreadSafeList.h test.cpp dryp1q3.cpp)