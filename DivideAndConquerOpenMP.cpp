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
#include <omp.h>

/**
 * The constructor
 * @param points
 */
DivideAndConquerOpenMP::DivideAndConquerOpenMP(vector<SmartPoint>& points) {
    build(points, false);

}

/**
 * The constructor
 * @param vec a vector of doubles where each point is made from three
 * consecutive doubles.
 */
DivideAndConquerOpenMP::DivideAndConquerOpenMP(const vector<double>& vec) {
    vector<SmartPoint> points;
    points.reserve(vec.size()/3);
    for(int i = 0; i < points.size() && vec[i] == vec[i]; i+=3)
        points.push_back(SmartPoint(vec, i));
    build(points, false);
}

/**
 * Builds the convex hull. This is kept separate from the constructors to 
 * assist with multi threading scope issues.
 * @param points the points the hull is to be built from.
 * @param alreadySorted true if the points are already sorted, false otherwise.
 */
void DivideAndConquerOpenMP::build(vector<SmartPoint>& points, bool alreadySorted) {
    if (!alreadySorted) sort(points.begin(), points.end());
    reserve(2 * points.size() * points.size());

    int numSubHulls = maxNumSubHulls(points);
    if (numSubHulls == 1) {
        IncrementalMethod im(points);
        importEntireHull(im);
        return;
    }

    DivideAndConquerOpenMP left, right;
    omp_set_num_threads(2);
#pragma omp parallel
    {
        bool fewThreads = omp_get_num_threads() < 2;

        if (omp_get_thread_num() == 0 || fewThreads) {
            vector<SmartPoint> leftPoints(points.begin(), points.begin() + points.size() / 2);
            left.build(leftPoints, true);
        }
        if (omp_get_thread_num() == 1 || fewThreads) {
            vector<SmartPoint> rightPoints(points.begin() + points.size() / 2, points.end());
            right.build(rightPoints, true);
        }

    }
    GiftWrapping gw(left, right);
    importEntireHull(gw);
}

