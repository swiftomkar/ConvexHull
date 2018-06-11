#pragma once
#include "Point.h"
#include <vector>

class SmartFacet;
using namespace std;

/**
 * @author Dov Neimand
 * This point keeps track of all the facets that face it.
 */
class SmartPoint: public Point {
public:
        
    vector<SmartFacet*> facingFacets;   
        
    SmartPoint(double x, double y, double z);
    SmartPoint(const vector<double>& vec, int index);
    SmartPoint(Point p);
    
    bool inside() const;
    
};
