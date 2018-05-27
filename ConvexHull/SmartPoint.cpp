/**
 * @author Dov Neimand 
 */
#include "SmartPoint.h"
#include "SmartFacet.h"

/**
 * Is this point inside of all the facets it's keeping track of?
 * @return true if it is, false otherwise
 */
bool SmartPoint::inside() const{
    return facingFacets.size() == 0;
}

