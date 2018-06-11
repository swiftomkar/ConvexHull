#include <unordered_set>

#include "SmartEdge.h"
#include "ConvexHull.h"

/**
 * This class is a convex hull.  It uses the incremental method.  The algorithm
 * is spelled out here: 
 * https://www2.cs.arizona.edu/classes/cs437/fall07/ch3d.prn.pdf
 * @author Dov Neimand
 */

class IncrementalMethod:public ConvexHull{
public:
    IncrementalMethod(vector<SmartPoint>& points);
    
private:
    void pyramid(const SmartPoint& p);
    void cone(const SmartPoint& p, vector<SmartEdge>& edges);
    void setHorizon(vector<SmartEdge>& horizon, const SmartPoint& star);
    void updateConflifctGraph(const vector<SmartEdge>& horizon, SmartPoint& star);
    unordered_set<SmartPoint*> potentialHullPoints;

    
};

