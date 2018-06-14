
/* 
 * File:   GiftWrapping.h
 * Author: Dov
 *
 * Created on May 27, 2018, 4:22 PM
 */

#pragma once

#include "ConvexHull.h"
#include "HullProj2d.h"
#include "SmartEdge.h"

/**
 * This class gift wraps to ready made convex hulls, a left hull and a right 
 * hull.  Every point in the left hull must be entirely to the left of 
 * every point in the right hull.
 * 
 * Note, this destroys the hulls that are used to assemble it.
 */
class GiftWrapping : public ConvexHull {
public:

    GiftWrapping() {
    }
    GiftWrapping(ConvexHull& left, ConvexHull& right);
    void build(ConvexHull& left, ConvexHull& right);
private:
    void supportingLine(const HullProj2d& projLeft, int& leftSupportingEdge,
            const HullProj2d& projRight, int& rightSupportingEdge) const;
    bool nextHorizon(SmartEdge& edge, const Point& star, bool counter) const;
    bool firstFacet(SmartEdge& leftEdge, SmartEdge& rightEdge);
    void addFacet(SmartEdge& edge, bool clockwise, SmartEdge& farEdge);
    void buildBridge(ConvexHull& a, ConvexHull& b);
    void initEdges(SmartEdge& left, SmartEdge& right,
            ConvexHull& chLeft, ConvexHull& chRight);
    const bool CLOCKWISE = false, COUNTER = true;
    void gut(SmartFacet& start);
    void setNeighbors();
    bool endBridge();
    void bondBaseNeighbors();
    void bondBasesForFlatNeighbors(SmartFacet* f1, SmartFacet* f2);

    void buildHullProjections(HullProj2d& hpLeft, HullProj2d& hpRight,
            ConvexHull& chLeft, ConvexHull& chRight);
    /**
     * These are used to keep track of a sample point from the day area of
     * each hull;
     */
    SmartFacet* leftInside, * rightInside;
};
