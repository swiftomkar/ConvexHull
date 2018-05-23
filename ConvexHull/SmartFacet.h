#pragma once

#include <vector>
#include "Facet.h"

class SmartPoint;

class SmartFacet: public Facet {
public:    
    
    void bondPoint(SmartPoint& p);
    void initPoints(std::vector<SmartPoint>& points, int start);
    void decouple();
    
    void bondNieghbor(int i, SmartFacet& f);
    void disable();
    bool enabled();
    
    SmartFacet* getNeigbor(int i);
    
private:
    friend SmartPoint;
    std::vector<SmartPoint*> points;    
    bool isEnabled = true;
    std::vector<SmartFacet*> neighbors;
    
};
