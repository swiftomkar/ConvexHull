
/* 
 * File:   ConvexHull.h
 * Author: eneimand
 *
 * Created on April 9, 2018, 2:07 PM
 */

#ifndef CONVEXHULL_H
#define CONVEXHULL_H

#include "Facet.h"
#include <vector>


class ConvexHull : public std::vector<Facet>{
public:
    ConvexHull(std::vector<Point> points);
    
//    ConvexHull(const ConvexHull& orig);
//    virtual ~ConvexHull();
private:
    

};

#endif /* CONVEXHULL_H */

