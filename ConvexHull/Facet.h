
/* 
 * File:   Facet.h
 * Author: dov
 * a list of points counterclockwise
 * Created on April 2, 2018, 12:14 PM
 */

#ifndef FACET_H
#define FACET_H

#include "Point.h"


class Facet: public vector<Point>{

public:
    
    bool faces(Point p);
};

#endif /* FACET_H */

