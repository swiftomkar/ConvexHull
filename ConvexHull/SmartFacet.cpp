#include "SmartFacet.h"
#include "SmartPoint.h"


void SmartFacet::bondPoint(SmartPoint& p) {
    points.push_back(&p);
    p.facets.push_back(this);
}

void SmartFacet::initPoints(std::vector<SmartPoint*>& points) {
    for (int i = 0; i < points.size(); i++)
        if (faces(*(points[i]))) 
            bondPoint(*points[i]);
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
            for (int j = 0; j < points[i]->facets.size(); j++)
                if (points[i]->facets[j] == this) {
                    points[i]->facets.erase(points[i]->facets.begin() + j);
                    break;
                }

}

void SmartFacet::bondNieghborOneSide(int i, SmartFacet& f) {
    while (i <= size()) neigbors.push_back(nullptr);
    neigbors[i] = &f;
}

void SmartFacet::disable() {
    isEnabled = false;
}

bool SmartFacet::enabled() {
    return isEnabled;
}


