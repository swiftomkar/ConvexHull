

#include "ConvexHull.h"
#include "SmartPoint.h"
#include "Edge.h"

using namespace std;

ConvexHull::ConvexHull(vector<SmartPoint> points) {

    int pointIndex = firstFacet(points);
    pyramid(points[pointIndex], (*this)[0]);

    initConflictGraph(vector<SmartPoint>(points.begin() + pointIndex + 1, points.begin() + points.size()));

    while (++pointIndex < points.size()) {
        while (pointIndex < points.size() && points[pointIndex].inside()) pointIndex++;
        if (pointIndex == points.size()) break;

        vector<Edge> horizon;
        setHorizon(horizon, points[pointIndex]);
        cone(points[pointIndex], horizon);
        removeFace(points[pointIndex]);
        updateConflifctGraph(horizon, points[pointIndex]);
    }

}

int ConvexHull::firstFacet(vector<SmartPoint> points) {
    SmartFacet facet1;
    int pointsIndex = 0;
    facet1.push_back(points[pointsIndex++]);

    //make a line    
    for (; pointsIndex < points.size() && (points[pointsIndex] == points[0]); pointsIndex++);
    facet1.push_back(points[pointsIndex++]);


    //make a triangle
    for (; pointsIndex < points.size(); pointsIndex++) {
        int j = colinear(facet1[0], facet1[1], points[pointsIndex]);
        if (j == -1) break;
        facet1.erase(facet1.begin() + j);
    }
    facet1.push_back(points[pointsIndex++]);

    for (; pointsIndex < points.size(); pointsIndex++) {
        if (facet1.onPlane(points[pointsIndex])) facet1.addPoint(points[pointsIndex]);
    }
    push_back(facet1);

    if (pointsIndex >= size()) throw "no polyhedron can be made from these points.";
    return pointsIndex;
}

void ConvexHull::pyramid(SmartPoint& tip, SmartFacet& base) {

    if (base.faces(tip)) base.flip();

    for (int j = 0; j < base.size(); j++) {
        SmartFacet side;
        side.push_back(base[(j + 1) % base.size()]);
        side.push_back(base[j]);
        side.push_back(tip);

        base.bondNieghborOneSide(j, side);
        side.bondNieghborOneSide(0, base);
        if (j > 0) {
            side.bondNieghborOneSide(1, (*this)[size() - 1]);
            (*this)[size() - 1].bondNieghborOneSide(2, side);
        }
        side.bondNieghborOneSide(2, (*this)[size() - base.size()]);
        (*this)[size() - base.size()].bondNieghborOneSide(1, side);

        push_back(side);
    }

}

void ConvexHull::cone(SmartPoint& tip, std::vector<Edge>& edges){
    for (int j = 0; j < edges.size(); j++) {
        SmartFacet side;
        side.push_back(edges[j].b());
        side.push_back(edges[j].a());
        side.push_back(tip);

        edges[j].outside()->bondNieghborOneSide(edges[j].outsideIndex(), side);
        side.bondNieghborOneSide(0, *(edges[j].outside()));
        if (j > 0) {
            side.bondNieghborOneSide(1, (*this)[size() - 1]);
            (*this)[size() - 1].bondNieghborOneSide(2, side);
        }
        side.bondNieghborOneSide(2, (*this)[size() - edges.size()]);
        (*this)[size() - edges.size()].bondNieghborOneSide(1, side);

        push_back(side);
    }
}


void ConvexHull::initConflictGraph(vector<SmartPoint> points) {
    vector<SmartPoint*> smartPointPointers;
    for(int i = 0; i < points.size(); i++)
        smartPointPointers.push_back(&points[i]);
    for (int i = 0; i < size(); i++)
        (*this)[i].initPoints(smartPointPointers);
}

void ConvexHull::setHorizon(vector<Edge>& horizon, SmartPoint& star) {
    
    Edge search(0, star.facets[0]);
    
    while(search.outside()->faces(star)) {
        search.flip();
        search.changeDir();
        search.progress();        
    }
    
    horizon.push_back(Edge(search));
    
    for(search.progress(); !(search == horizon[0]); search.progress()){
        if(search.outside()->faces(star)){
            search.flip();
            search.progress();
        }
        horizon.push_back(Edge(search));
    }    
}

void ConvexHull::removeFace(SmartPoint& star) {
    for(int i = 0; i < star.facets.size(); i++) {
        (*star.facets[i]).disable();
    }
}

void ConvexHull::updateConflifctGraph(vector<Edge>& horizon, SmartPoint& star) {
    vector<SmartPoint> starbin;
    for(int i = 0; i < star.facets.size(); i++){
        for(int j = 0; j < (*star.facets[i]).size(); j++){
            starbin.push_back((*star.facets[i])[j]);
        }
        (*star.facets[i]).decouple();
    }
    for(int i = 0; i < horizon.size(); i++){
        for(int j = 0; j <horizon[i].outside()->size(); j++){
            starbin.push_back((*horizon[i].outside())[j]);
        }
    }
    
    for(int i = 0; i < horizon.size(); i++)
        for(int j = 0; j < starbin.size(); j++)
            if((*this)[size() - i].faces(starbin[j])) {
                starbin[j].bondFacet((*this)[size() - i]);
            }
}

void ConvexHull::removeDisabledFacets() {
    int shift = 0;
    for(int i = 0; i < size() - shift; i++){
        if(!(*this)[i].enabled()) shift++;
        (*this)[i] = (*this)[i + shift];
    }
    for(int i = 0; i < shift; i++) pop_back();
        
}
