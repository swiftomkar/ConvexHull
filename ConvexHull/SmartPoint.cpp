#include "SmartPoint.h"
#include "SmartFacet.h"

void SmartPoint::bondFacet(SmartFacet& f) {
    facingFacets.push_back(&f);
    f.facingPoints.push_back(this);
}

void SmartPoint::decouple() {
    for(int i = 0; i < facingFacets.size(); i++)
        for(int j = 0; j < facingFacets[i]->facingPoints.size(); j++)
            if(facingFacets[i]->facingPoints[j] == this) {
                facingFacets[i]->decouple();
                facingFacets[i]->disable();
                break;
            }
}

bool SmartPoint::inside() const{
    return facingFacets.size() == 0;
}

