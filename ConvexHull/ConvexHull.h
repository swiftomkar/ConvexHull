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
    friend ostream& operator << (ostream& os, ConvexHull ch);
    
private:
    void pyramid(SmartPoint& p);
    void cone(SmartPoint& p, vector<Edge>& edges);
    int firstFacet(vector<SmartPoint> points);    
    void setHorizon(vector<Edge>& horizon, SmartPoint& star);
    void updateConflifctGraph(vector<Edge>& horizon, SmartPoint& star);
    void removeDisabledFacets();

};
