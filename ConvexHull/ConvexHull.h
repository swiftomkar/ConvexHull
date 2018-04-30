#pragma once

#include "Facet.h"
#include "SmartFacet.h"
#include "Edge.h"
#include <vector>

using namespace std;

class ConvexHull : public vector<SmartFacet>{
public:
    ConvexHull(vector<SmartPoint> points);    
    
//    ConvexHull(const ConvexHull& orig);
//    virtual ~ConvexHull();
    
private:
    void pyramid(SmartPoint& p, SmartFacet& f);
    void cone(SmartPoint& p, vector<Edge>& edges);
    int firstFacet(vector<SmartPoint> points);
    void initConflictGraph(vector<SmartPoint> points);
    void setHorizon(vector<Edge>& horizon, SmartPoint& star);
    void removeFace(SmartPoint& star);
    void updateConflifctGraph(vector<Edge>& horizon, SmartPoint& star);
    void removeDisabledFacets();
    
};
