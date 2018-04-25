/* 
 * File:   smartPoint.h
 * Author: dov
 *
 * Created on April 25, 2018, 5:24 PM
 */

#ifndef SMARTPOINT_H
#define SMARTPOINT_H

#include <vector>

#include "SmartFacet.h"


class SmartPoint: public Point {
public:
    
    void bondFacet(const SmartFacet& f);

    SmartPoint(double x, double y, double z):Point(x, y, z) {}
    SmartPoint(Point p): x(p.x), y(p.y), z(p.z){}
    
    void decouple();
    bool inside();
    
private:
    std::vector<SmartFacet> facets;
    friend SmartFacet;
};

#endif /* SMARTPOINT_H */

