#include "SmartFacet.h"
#include "SmartPoint.h"

void SmartFacet::bondPoint(SmartPoint& p) {
    sky.push_back(&p);
    p.facingFacets.push_back(this);
    std::cout << "bonding " << *this << " to " << p << endl;
}

void SmartFacet::initPoints(std::vector<SmartPoint>& points, int start) {
    for (int i = start; i < points.size(); i++)
        if (faces(points[i])) bondPoint(points[i]);
}

void SmartFacet::bondNieghbor(int side, SmartFacet & f) {
    while (side >= neighbors.size())
        neighbors.push_back(nullptr);
    neighbors[side] = &f;
}

void SmartFacet::disable() {
    isEnabled = false;
    for (int i = 0; i < sky.size(); i++) {
        int shift = 0;
        
        for (int j = 0; j + shift < sky[i]->facingFacets.size(); j++) {
            if (sky[i]->facingFacets[j + shift] == this){
                shift++; j--;
                continue;
            }
            sky[i]->facingFacets[j] = sky[i]->facingFacets[j + shift];
        }
        for(int j = 0;j < shift; j++) 
            sky[i]->facingFacets.pop_back();
    }
}

bool SmartFacet::enabled() {
    return isEnabled;
}

SmartFacet * SmartFacet::getNeighbor(int i) {
    return neighbors[i];
}

SmartFacet::SmartFacet(int expectedSize) {
    reserve(expectedSize);
}
