#pragma once

#include <vector>
#include "Point.h"
#include <ostream>

class Facet: public std::vector<Point> {
public: 

    bool faces(const Point& p) const;
    bool onPlane(const Point& p) const;
    Point normal() const;
    Facet sidePlane(int i) const;
    bool edgeFaces(int edge, const Point& p) const;
    void addPoint(const Point& p);
    void flip();
    
    
    bool operator ==(const Facet& f) const;
    friend std::ostream& operator <<(std::ostream& os, Facet f);

};

