/* 
 * File:   smartPoint.h
 * Author: dov
 *
 * Created on April 25, 2018, 5:24 PM
 */

#ifndef SMARTPOINT_H
#define SMARTPOINT_H

#include "SmartFacet.h"
#include <vector>

using namespace SmartPoint;

class SmartPoint: public Point {
public:
        
    SmartPoint::vector<SmartFacet> facets;
    
    void bondFacet(const SmartFacet& f);

    SmartPoint(double x, double y, double z):Point(x, y, z) {}
    SmartPoint(Point p): Point(p){}
    
    void decouple();
    bool inside();
    
};

#endif /* SMARTPOINT_H */

