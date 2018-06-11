/**
 * @author Dov Neimand
 * This file contains the main and tests the methods.
 */

#include <cstdlib>
#include "ConvexHull.h"
#include "SmartPoint.h"
#include "IncrementalMethod.h"
#include "HullProj2d.h"
#include "GiftWrapping.h"
#include <algorithm>
#include <cmath>
#include "Test.h"
using namespace std;

int ConvexHull::stlPrint = 0;

int main(int argc, char** argv) {
    Test t;
    t.incrementalMethod2(10);
//    t.divideAndConquerOpenMP3(50);
    return 0;
}

