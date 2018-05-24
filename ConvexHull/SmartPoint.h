#pragma once

#include "Point.h"
#include <vector>


class SmartFacet;

using namespace std;

class SmartPoint: public Point {
public:
        
    vector<SmartFacet*> facingFacets;   
        
    void bondFacet(SmartFacet& f);

    SmartPoint(double x, double y, double z):Point(x, y, z) {}
    SmartPoint(Point p): Point(p){}
    
    void decouple();
    bool inside() const;
    
};
