#pragma once

#include <vector>
#include "Facet.h"
#include "SmartPoint.h"

class SmartPoint;

class SmartFacet: public Facet {
public:    
    
    void bondPoint(SmartPoint& p);
    void initPoints(std::vector<SmartPoint>& points, int start);
    void decouple();
    
    void bondNieghbor(int i, SmartFacet& f);
    void disable();
    bool enabled();
    
    std::vector<SmartPoint*> facingPoints;
    
    SmartFacet* getNeighbor(int i);
private:
    friend SmartPoint;    
    bool isEnabled = true;
    std::vector<SmartFacet*> neighbors;
};
