/* 
 * File:   ConvexHull.cpp
 * Author: eneimand
 * 
 * Created on April 9, 2018, 2:07 PM
 */

#include "ConvexHull.h"
#include "smartPoint.h"

ConvexHull::ConvexHull(std::vector<SmartPoint> points) {

    int i = firstFacet(points);
    pyramid(points[i], (*this)[0]);

    initConflictGraph(std::vector<SmartPoint>(points.begin() + i + 1, points.begin() + points.size()));


    while (++i > points.size()) {
        while(i < points.size() && points[i].inside()) i++;
        if(i == points.size()) break;
        
        
    }

}

int ConvexHull::firstFacet(std::vector<SmartPoint> points) {
    SmartFacet facet1;
    int pointsIndex = 0;
    facet1.push_back(points[pointsIndex++]);

    //make a line    
    for (; pointsIndex < points.size() && (points[pointsIndex] == points[0]); pointsIndex++);
    facet1.push_back(points[pointsIndex++]);


    //make a triangle
    for (; pointsIndex < points.size(); pointsIndex++) {
        int j = colinear(facet1[0], facet1[1], points[pointsIndex]);
        if (j == -1) break;
        facet1.erase(facet1.begin() + j);
    }
    facet1.push_back(points[pointsIndex++]);

    for (; pointsIndex < points.size(); pointsIndex++) {
        if (facet1.onPlane(points[pointsIndex])) facet1.addPoint(points[pointsIndex]);
    }
    push_back(facet1);

    if (pointsIndex >= size()) throw "no polyhedron can be made from these points.";
    return pointsIndex;
}

void ConvexHull::pyramid(const SmartPoint& tip, SmartFacet& base) {

    if (base.faces(tip)) base.flip();

    for (int j = 0; j < base.size(); j++) {
        SmartFacet side;
        side.push_back(base[(j + 1) % base.size()]);
        side.push_back(base[j]);
        side.push_back(tip);

        base.bondNieghborOneSide(j, side);
        side.bondNieghborOneSide(0, base);
        if (j > 0) {
            side.bondNieghborOneSide(1, (*this)[size() - 1]);
            (*this)[size() - 1].bondNieghborOneSide(2, side);
        }
        side.bondNieghborOneSide(2, (*this)[size() - base.size()]);
        (*this)[size() - base.size()].bondNieghborOneSide(1, side);
        
        push_back(side);
    }

}

void ConvexHull::initConflictGraph(std::vector<SmartPoint> points) {
    for(int i = 0; i < size(); i++)
        (*this)[i].initPoints(points);
}















//
//ConvexHull::ConvexHull(const ConvexHull& orig) {
//}
//
//ConvexHull::~ConvexHull() {
//}

