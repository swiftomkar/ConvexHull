
#pragma once
#include <vector>
#include "Point.h"
#include <ostream>

/**
 * @author Dov Neimand
 * Describes a facet or polygon in 3d space.  The points must be in counter 
 * clockwise order and convex.
 */
class Facet: public std::vector<Point> {
public: 

    bool faces(const Point& p) const;
    bool onPlane(const Point& p) const;
    Point normal() const;
    Facet sidePlane(int i) const;
    bool edgeFaces(int edge, const Point& p) const;
    void addPoint(const Point& p);
    void flip();
    
    friend std::ostream& operator <<(std::ostream& os, Facet f);

};

