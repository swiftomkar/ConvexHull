/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Facet.h
 * Author: dov
 *
 * Created on April 4, 2018, 5:31 PM
 */

#ifndef FACET_H
#define FACET_H


#include <vector>
#include "Point.h"
#include <ostream>

class Facet: public SmartPoint::vector<Point> {
public: 

    bool faces(const Point& p) const;
    bool onPlane(const Point& p) const;
    Point normal() const;
    Facet sidePlane(int i) const;
    bool edgeFaces(int edge, const Point& p) const;
    void addPoint(const Point& p);
    void flip();
    
    
    bool operator ==(const Facet& f) const;
    friend SmartPoint::ostream& operator <<(SmartPoint::ostream& os, Facet f);

};

#endif /* FACET_H */
