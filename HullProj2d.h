
/* 
 * File:   HullProj2d.h
 * Author: Dov
 *
 * Created on May 28, 2018, 9:03 AM
 */

#pragma once

#include "Point.h"
#include "ConvexHull.h"
#include <vector>

using namespace std;
/**
 * @author Dov Neimand
 * Returns a list of points on the 2d convex hull in counter-clockwise order.
 * @param ch
 */
class HullProj2d: public Facet{
public:
    HullProj2d(ConvexHull& ch);
    int minX();
    int maxX();
    Facet sidePlane(int i) const;
    bool edgeFaces(int edge, const Point& p) const;
private:
    double cross2d(const Point &O, const Point &A, const Point &B);
};
