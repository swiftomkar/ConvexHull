
/* 
 * File:   DivideAndConquer.h
 * @author: Dov
 *
 * Created on June 6, 2018, 10:06 AM
 */

#pragma once

#include "ConvexHull.h"

using namespace std;

/**
 * This class uses openMP to split a set of points into two sets, process 
 * each of them with a separate thread and incremental method, then merge them
 * with gift wrapping. 
 */
class DivideAndConquerOpenMP : public ConvexHull {
public:

    DivideAndConquerOpenMP(){}
    DivideAndConquerOpenMP(vector<SmartPoint>& points);
    DivideAndConquerOpenMP(const vector<double>& points);
    void build(vector<SmartPoint>& points, bool alreadySorted);
private:
};
