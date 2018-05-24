#include "SmartFacet.h"
#include "SmartPoint.h"


void SmartFacet::bondPoint(SmartPoint& p) {
    points.push_back(&p);
    p.facingFacets.push_back(this);
}

void SmartFacet::initPoints(std::vector<SmartPoint>& points, int start) {
    for (int i = start; i < points.size(); i++)
        if (faces(points[i])) 
            bondPoint(points[i]);
}

void SmartFacet::decouple() {//I'm not sure I need this.
//    for (int i = 0; i < neigbors.size(); i++)
//        if (!neigbors[i].faces(source))  //if the neibor does not face the source, then remove this.
//            for (int j = 0; j < neigbors[i].neigbors.size(); j++)
//                if (neigbors[i].neigbors[j] == *this) {
//                    neigbors[i].neigbors.erase(neigbors[i].neigbors.begin() + j);
//                    break;
//                }

        for (int i = 0; i < points.size(); i++)
            for (int j = 0; j < points[i]->facingFacets.size(); j++)
                if (points[i]->facingFacets[j] == this) {
                    points[i]->facingFacets.erase(points[i]->facingFacets.begin() + j);
                    break;
                }

}

void SmartFacet::bondNieghbor(int side, SmartFacet& f) {
    while (side >= neighbors.size()) 
        neighbors.push_back(nullptr);
    neighbors[side] = &f;
}

void SmartFacet::disable() {
    isEnabled = false;
}

bool SmartFacet::enabled() {
    return isEnabled;
}

SmartFacet* SmartFacet::getNeigbor(int i) {
    return neighbors[i];
}

