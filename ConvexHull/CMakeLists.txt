cmake_minimum_required(VERSION 3.9)
project(GrowArray)

set(CMAKE_CXX_STANDARD 11)

add_executable(ConvexHull main.cpp ConflictGraph.h Point.h Point.cpp Facet.h Facet.cpp)