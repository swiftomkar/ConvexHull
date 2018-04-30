#pragma once

#include <vector>
#include "Facet.h"

class SmartPoint;

class SmartFacet: public Facet {
public:
    std::vector<SmartFacet*> neigbors;
    
    void bondPoint(SmartPoint& p);
    void initPoints(std::vector<SmartPoint*>& points);
    void decouple();
    
    void bondNieghborOneSide(int i, SmartFacet& f);
    void disable();
    bool enabled();
    
private:
    friend SmartPoint;
    std::vector<SmartPoint*> points;    
    bool isEnabled = true;
    
};
