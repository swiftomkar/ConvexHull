
/* 
 * File:   Test.h
 * Author: Dov
 *
 * Created on June 6, 2018, 7:25 AM
 */

#pragma once


#include <cstdlib>
#include "ConvexHull.h"
#include "SmartPoint.h"
#include "HullProj2d.h"
#include "GiftWrapping.h"
#include <algorithm>

class Test {
public:

    void incrementalMethod1();
    void incrementalMethod2(int numPoints, int seed, bool mute);
    void giftWrapping1();
    void giftWrapping2();
    void giftWrapping3();
    void giftWrapping4();
    void giftWrapping5();
    void giftWrapping6(int size, int seed, bool mute);
    void divideAndConquerOpenMP1();
    void divideAndConquerOpenMP2();
    void divideAndConquerOpenMP3(int numPoints, int seed, bool mute);
    void writeToAndReadFromArray();
    void testEverything();

private:

};


