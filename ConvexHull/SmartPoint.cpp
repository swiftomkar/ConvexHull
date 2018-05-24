#include "SmartPoint.h"
#include "SmartFacet.h"

void SmartPoint::bondFacet(SmartFacet& f) {
    facingFacets.push_back(&f);
    f.points.push_back(this);
}

void SmartPoint::decouple() {
    for(int i = 0; i < facingFacets.size(); i++)
        for(int j = 0; j < facingFacets[i]->points.size(); j++)
            if(facingFacets[i]->points[j] == this) {
                facingFacets[i]->decouple();
                facingFacets[i]->disable();
                break;
            }
}

bool SmartPoint::inside() {
    return facingFacets.size() == 0;
}

