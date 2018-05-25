#include "SmartPoint.h"
#include "SmartFacet.h"


bool SmartPoint::inside() const{
    return facingFacets.size() == 0;
}

