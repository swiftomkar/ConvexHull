cmake_minimum_required(VERSION 3.9)
project(myConvexHull)

set(CMAKE_CXX_STANDARD 11)

add_executable(myConvexHull main.cpp ConflictGraph.h ConvexHull.h ConvexHull.cpp Facet.h Facet.cpp Point.h Point.cpp ConflictGraph.cpp GiftWrap.cpp GiftWrap.h)