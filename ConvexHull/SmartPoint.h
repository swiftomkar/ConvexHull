#pragma once

#include "Point.h"
#include <vector>


class SmartFacet;

using namespace std;

class SmartPoint: public Point {
public:
        
    vector<SmartFacet*> facingFacets;   
        
    SmartPoint(double x, double y, double z):Point(x, y, z) {}
    SmartPoint(Point p): Point(p){}
    
    bool inside() const;
    
};
