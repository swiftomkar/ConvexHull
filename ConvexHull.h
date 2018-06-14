#pragma once
#include <vector>
#include "SmartFacet.h"

using namespace std;

/**
 * This class is the parent class for convex hulls.
 * It can also build a convex hull from a vector of doubles arranged to
 * descrive a CH.
 * @author Dov Neimand
 */
class ConvexHull : public vector<SmartFacet> {
public:
    static int stlPrint;

    ConvexHull() {
    };

    ConvexHull(const vector<double>& vec, int& endIndex);
    
    friend ostream& operator<<(ostream& os, ConvexHull& ch);
    void locPoint(const Point& search, int& facetInd, int& pointInd) const;

    bool testNeighbors() const;
    bool isConvex(bool mute);
    
    void stl();

    void writeToVector(vector<double>& vec);
    
    Point getHullPoint(int i) const;
    int numHullPoints()const;
    void sortHullPoints();
protected:
    vector<Point> hullPoints;
    int findFirstFacet(const vector<SmartPoint>& points, int start, bool add);
    void importEntireHull(ConvexHull& source);
    void removeDisabledFacets();
    void setHullPoints();
    void importFacets(ConvexHull& source);
    int maxNumSubHulls(const vector<SmartPoint>& points);
};
