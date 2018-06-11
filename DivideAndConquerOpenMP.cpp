/* 
 * File:   DivideAndConquer.cpp
 * Author: Dov
 * 
 * Created on June 6, 2018, 10:06 AM
 */
//use open mp

#include "DivideAndConquerOpenMP.h"
#include "IncrementalMethod.h"
#include "GiftWrapping.h"
#include "DivideAndConquerOpenMP.h"
#include <algorithm>

/**
 * The constructor
 * @param points
 */
DivideAndConquerOpenMP::DivideAndConquerOpenMP(vector<SmartPoint>& points) 
: DivideAndConquerOpenMP(points, false) {

}

DivideAndConquerOpenMP::DivideAndConquerOpenMP(vector<SmartPoint>& points, bool alreadySorted) {
    if (!alreadySorted) sort(points.begin(), points.end());
    reserve(2 * points.size() * points.size());

    int numSubHulls = maxNumSubHulls(points);
    if (numSubHulls == 1) {
        IncrementalMethod im(points);
        importEntireHull(im);
        return;
    }

    /////////////////done in parallel//////////
    vector<SmartPoint> leftPoints(points.begin(), points.begin() + points.size() / 2);
    vector<SmartPoint> rightPoints(points.begin() + points.size() / 2, points.end());

    if (numSubHulls == 2) {
        IncrementalMethod leftHull(leftPoints);
        IncrementalMethod rightHull(rightPoints);
        
        leftHull.stl();
        rightHull.stl();
        //////////////////end parrallel//////////////
        GiftWrapping gw(leftHull, rightHull);
        importEntireHull(gw);
        return;
    }

    DivideAndConquerOpenMP leftHull(leftPoints, true);
    DivideAndConquerOpenMP rightHull(rightPoints, true);
    //////////////////end parralell//////////////
    GiftWrapping gw(leftHull, rightHull);
    importEntireHull(gw);
    return;
}

/**
 * Does this set of points allow for 0, 1 or two sub hulls?  That is, if we
 * split the set of points in half, does each half have enough points to make
 * a 3d convex hull.
 * @param points the points the sub hulls are to be taken from.
 * @return 0, 1, 2, or 4 depending on weather or not hulls can be made when the
 * set is cut in half.
 */
int DivideAndConquerOpenMP::maxNumSubHulls(const vector<SmartPoint>& points) {
    int firstFacetSize = findFirstFacet(points, 0, false) + 1;
    if (firstFacetSize >= points.size()) {
        cerr << "DivideAndConquer::DivideAndConquer " <<
                "points can't make polyhedron.";
        throw "DivideAndConquer::DivideAndConquer points can't make polyhedron.";
    }
    int secondHalfFacetSize = findFirstFacet(points, points.size() / 2, false) + 1;
    if (secondHalfFacetSize >= points.size() || firstFacetSize >= points.size() / 2) {
        return 1;
    }
    if (firstFacetSize > points.size() / 4 && secondHalfFacetSize > 3 * points.size() / 4) {
        return 2;
    }

    if (firstFacetSize < points.size() / 4 && secondHalfFacetSize < points.size() * 3.0 / 4
            && findFirstFacet(points, points.size() / 4, false) + 1 < points.size() / 2
            && findFirstFacet(points, 3 * points.size() / 4, false) + 1 < points.size())
        return 4;
    return 2;
}
