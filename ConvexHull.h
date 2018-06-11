#pragma once
#include <vector>
#include "SmartFacet.h"

using namespace std;

/**
 * This class is the parent class for convex hulls.
 * @author Dov Neimand
 */
class ConvexHull : public vector<SmartFacet> {
public:
    static int stlPrint;

    ConvexHull() {
    };

    ConvexHull(const vector<double>& vec);
    vector<Point> hullPoints;

    friend ostream& operator<<(ostream& os, ConvexHull& ch);
    void locPoint(const Point& search, int& facetInd, int& pointInd) const;

    bool testNeighbors();
    bool isConvex(bool mute);
    void importFacets(ConvexHull& source);
    void stl();

    void writeToVector(vector<double>& vec);
protected:
    int findFirstFacet(const vector<SmartPoint>& points, int start, bool add);
    void importEntireHull(ConvexHull& source);
    void removeDisabledFacets();
    void setHullPoints();

};
