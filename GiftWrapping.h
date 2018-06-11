
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
 */
class GiftWrapping : public ConvexHull {
public:
    GiftWrapping(ConvexHull& left, ConvexHull& right);
private:
    void supportingLine(HullProj2d& projLeft, int& leftSupportingEdge,
            HullProj2d& projRight, int& rightSupportingEdge);
    bool nextHorizon(SmartEdge& edge, Point star, bool counter);
    void neighborLastToFirst();
    
    void addFacet(SmartEdge& edge, bool clockwise, SmartEdge& farEdge);
    void buildBridge(ConvexHull& a, ConvexHull& b);
    void initEdges(SmartEdge& left, SmartEdge& right,
            ConvexHull& chLeft, ConvexHull& chRight);
    const bool CLOCKWISE = false, COUNTER = true;
    void gut(SmartFacet& start);
    void setNeighbors(SmartEdge& edge);
    void removeLast(SmartEdge& edge);
    bool endBridge();
    void bondFlatNeighbors();
    void bondBasesForFlatNeigbors(SmartFacet& f1, SmartFacet& f2);
    SmartFacet* leftInside,* rightInside;
};
