
/* 
 * File:   ConvexHull.h
 * Author: eneimand
 *
 * Created on April 9, 2018, 2:07 PM
 */

#ifndef CONVEXHULL_H
#define CONVEXHULL_H

#include "Facet.h"
#include "SmartFacet.h"
#include <vector>


class ConvexHull : public std::vector<SmartFacet>{
public:
    ConvexHull(std::vector<SmartPoint> points);    
    
//    ConvexHull(const ConvexHull& orig);
//    virtual ~ConvexHull();
    void pyramid(const SmartPoint& p, SmartFacet& f);
private:
    int firstFacet(std::vector<SmartPoint> points);
    void initConflictGraph(std::vector<SmartPoint> points);
    
};

#endif /* CONVEXHULL_H */

