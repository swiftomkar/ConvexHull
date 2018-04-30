
/* 
 * File:   Edge.h
 * Author: dov
 *
 * Created on April 28, 2018, 5:51 PM
 */

#ifndef EDGE_H
#define EDGE_H

#include "SmartFacet.h"
#include "Point.h"


class Edge {
public:
    
    const SmartFacet* inside;
    
    
    Edge(int i, const SmartFacet& inside);
    Edge(const Edge& orig):index(orig.index), inside(orig.inside){}
    
    void progress();
    void flip();
    void changeDir();
    
    bool operator ==(const Edge& e)const;
    
    int outsideIndex() const;
    
    Point a() const;
    Point b() const;
    
    SmartFacet* outside() const;
    
private:
    int index;
    int indexDir = 1;

};

#endif /* EDGE_H */

