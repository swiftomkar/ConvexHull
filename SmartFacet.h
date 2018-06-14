#pragma once

#include <vector>
#include <cmath>
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
    
    SmartFacet(){}
    SmartFacet(int expectedSize);
    SmartFacet(const Point& a, const Point& b, const Point& c);
    
    void bondPoint(SmartPoint& p);
    void initPoints(std::vector<SmartPoint>& points, int start);
    
    void setNeighbor(int i, SmartFacet* f);
    void disable();
    bool enabled() const;
    
    
    SmartFacet* getNeighbor(int i);
    
    void changeOfAddress(int shift);
    void changeOfAddress(SmartFacet* changeTo);

    void copy(const SmartFacet& source);
    
    int neighborIndex(const SmartFacet* f) const;
    bool testNeighbors() const;
    int pointIndex(const Point& p) const;
    
    void appendToVector(std::vector<double>& vec) const;
    SmartFacet(const std::vector<double>& vec, int& vecIndex, SmartFacet* address);
    
    bool counterCl() const;
    
    SmartPoint* star(int i);
    int numStars() const;
    
private:
    bool isEnabled = true;
    std::vector<SmartFacet*> neighbors;
    std::vector<SmartPoint*> sky;
    
};
