/* 
 * File:   smartPoint.cpp
 * Author: dov
 * 
 * Created on April 25, 2018, 5:25 PM
 */

#include "smartPoint.h"

void SmartPoint::bondFacet(SmartFacet& f) {
    facets.push_back(f);
    f.points.push_back(*this);
}

void SmartPoint::decouple() {
    for(int i = 0; i < facets.size(); i++)
        for(int j = 0; j < facets[i].points.size(); j++)
            if(facets[i].points[j] == *this) {
                facets[i].decouple();
                facets[i].disable();
                break;
            }
    return facets;
}

bool SmartPoint::inside() {
    return facets.size() == 0;
}

