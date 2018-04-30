
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
#include "Edge.h"
#include <vector>


class ConvexHull : public SmartPoint::vector<SmartFacet>{
public:
    ConvexHull(SmartPoint::vector<SmartPoint> points);    
    
//    ConvexHull(const ConvexHull& orig);
//    virtual ~ConvexHull();
    
private:
    void pyramid(SmartPoint& p, SmartFacet& f);
    void cone(SmartPoint& p, SmartPoint::vector<Edge>& edges);
    int firstFacet(SmartPoint::vector<SmartPoint> points);
    void initConflictGraph(SmartPoint::vector<SmartPoint> points);
    void setHorizon(SmartPoint::vector<Edge>& horizon, SmartPoint& star);
    void removeFace(SmartPoint& star);
    void updateConflifctGraph(SmartPoint::vector<Edge>& horizon, SmartPoint& star);
    void removeDisabledFacets();
    
};

#endif /* CONVEXHULL_H */

