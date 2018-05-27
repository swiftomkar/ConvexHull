#pragma once
#include "Facet.h"
#include "SmartFacet.h"
#include "Edge.h"
#include <vector>

using namespace std;

/**
 * This class is a convex hull.  It uses the incremental method.  The algorithm
 * is spelled out here: 
 * https://www2.cs.arizona.edu/classes/cs437/fall07/ch3d.prn.pdf
 * @author Dov Neimand
 */
class ConvexHull : public vector<SmartFacet> {
public:
    ConvexHull(vector<SmartPoint>& points);
    friend ostream& operator << (ostream& os, const ConvexHull& ch);
    
private:
    void pyramid(const SmartPoint& p);
    void cone(const SmartPoint& p, vector<Edge>& edges);
    int firstFacet(const vector<SmartPoint>& points);    
    void setHorizon(vector<Edge>& horizon, const SmartPoint& star);
    void updateConflifctGraph(const vector<Edge>& horizon, SmartPoint& star);
    void removeDisabledFacets();
};
