#pragma once
#include "SmartFacet.h"
#include "Point.h"

/**
 * @author Dov Neimand
 * Describes an edge on the surface of the convex hull.
 * Since the edges are often used to generate new cones, this class also
 * can keep track of a replacement edges.
 */
class Edge {
public:
    
    SmartFacet* inside;
    SmartFacet* replacementInside;
    
    
    Edge(int i, SmartFacet* inside):index(i), inside(inside){}
    Edge(const Edge& orig):index(orig.index), inside(orig.inside){}
    
    void progress();
    void flip();
    void changeDir();
    
    bool operator ==(const Edge& e)const;
    
    int outsideIndex() const;
    
    Point a() const;
    Point b() const;
    
    SmartFacet* outside() const;
    
    friend std::ostream& operator <<(std::ostream& os, const Edge& e);
private:
    int index;
    int indexDir = 1;

};

