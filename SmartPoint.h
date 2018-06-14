#pragma once
#include "Point.h"
#include <algorithm>
#include "SmartFacet.h"
#include <vector>

class SmartFacet;
using namespace std;

/**
 * @author Dov Neimand
 * This point keeps track of all the facets that face it.
 */
class SmartPoint: public Point {
public:
        
    SmartPoint(double x, double y, double z);
    SmartPoint(const vector<double>& vec, int index);
    SmartPoint(Point p);
    
    bool inside() const;
    
    SmartFacet* getFaceingFacet(int i);;
    int numFacingFacets() const;
    void bondFacet(SmartFacet* facing);
    void removeFacingFacet(SmartFacet* facing);
    void reserveFacingFacetSpace(int size);
private:
    vector<SmartFacet*> facingFacets;
};
