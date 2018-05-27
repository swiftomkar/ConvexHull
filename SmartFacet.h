#pragma once

#include <vector>
#include "Facet.h"
#include "SmartPoint.h"

class SmartPoint;

/**
 * @author Dov Neimand
 * A facet that keeps track of its neighbors and the points that it faces.
 * An element of the conflict graph. 
 */
class SmartFacet: public Facet {
public:    
    
    SmartFacet(int expectedSize);
    
    void bondPoint(SmartPoint& p);
    void initPoints(std::vector<SmartPoint>& points, int start);
    
    void setNieghbor(int i, SmartFacet& f);
    void disable();
    bool enabled() const;
    
    std::vector<SmartPoint*> sky;
    
    SmartFacet* getNeighbor(int i);
    
private:
    friend SmartPoint;    
    bool isEnabled = true;
    std::vector<SmartFacet*> neighbors;
    
};
