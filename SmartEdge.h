#pragma once
#include "SmartFacet.h"
#include "Point.h"

/**
 * @author Dov Neimand
 * Describes an edge on the surface of the convex hull.
 * Since the edges are often used to generate new cones, this class also
 * can keep track of a replacement edges.
 */
class SmartEdge {
public:
    
    
    SmartEdge(int i, SmartFacet* inside):index(i), inside(inside){}
    SmartEdge(const SmartEdge& orig):index(orig.index), inside(orig.inside){}
    
    void progress();
    void flip();
    void changeDir();
    void backUp();
    
    bool operator ==(const SmartEdge& e)const;
    SmartEdge& operator =(const SmartEdge& e);
    
    int outsideIndex();
    
    int getIndex() const;
    void setIndex(int i);
    
    Point a() const;
    Point b() const;
    
    
    SmartFacet* getOutside() const;
    
    friend std::ostream& operator <<(std::ostream& os, const SmartEdge& e);
    int getIndexDir() const;
    
    SmartFacet* getInside();
    void setInside(SmartFacet* f);
    SmartFacet* getReplacementInside();
    void setReplacementInside(SmartFacet* f);
    
private:
    int index;
    int indexDir = 1;
    
    SmartFacet* inside;
    SmartFacet* replacementInside;
};