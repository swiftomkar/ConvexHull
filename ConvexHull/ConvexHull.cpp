

#include "ConvexHull.h"
#include "SmartPoint.h"
#include "Edge.h"

using namespace std;

ConvexHull::ConvexHull(vector<SmartPoint>& points) {

    int pointIndex = firstFacet(points);
    pyramid(points[pointIndex++]);

    for (int i = 0; i < size(); i++) 
        (*this)[i].initPoints(points, pointIndex);

        for (;pointIndex < points.size(); pointIndex++) {
            if (pointIndex < points.size() && points[pointIndex].inside()) continue;
     
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
        else {
            break;
        }
    }
    push_back(facet1);

    if (pointsIndex >= points.size()) throw "no polyhedron can be made from these points.";
    return pointsIndex;
}

void ConvexHull::pyramid(SmartPoint& tip) {

    if ((*this)[0].faces(tip)) (*this)[0].flip();

    for (int j = 0; j < (*this)[0].size(); j++) {
        SmartFacet side;
        push_back(side);

        (*this)[size() - 1].push_back((*this)[0][(j + 1) % (*this)[0].size()]);
        (*this)[size() - 1].push_back((*this)[0][j]);
        (*this)[size() - 1].push_back(tip);

        (*this)[0].bondNieghbor(j, (*this)[size() - 1]);
        (*this)[size() - 1].bondNieghbor(0, (*this)[0]);
        if (j > 0) {
            (*this)[size() - 1].bondNieghbor(1, (*this)[size() - 2]);
            (*this)[size() - 1].bondNieghbor(2, (*this)[size() - 1]);
        }


        //        (*this)[j+1] = side;
    }
    (*this)[size() - 1].bondNieghbor(2, (*this)[size() - (*this)[0].size()]);
    (*this)[size() - (*this)[0].size()].bondNieghbor(1, (*this)[size() - 1]);

}

void ConvexHull::cone(SmartPoint& tip, std::vector<Edge>& edges) {
    for (int j = 0; j < edges.size(); j++) {
        SmartFacet facet;
        push_back(facet);
        (*this)[size() - 1].push_back(edges[j].b());
        (*this)[size() - 1].push_back(edges[j].a());
        (*this)[size() - 1].push_back(tip);

        edges[j].outside()->bondNieghbor(edges[j].outsideIndex(), (*this)[size() - 1]);
        (*this)[size() - 1].bondNieghbor(0, *(edges[j].outside()));
        if (j > 0) {
            (*this)[size() - 1].bondNieghbor(1, (*this)[size() - 2]);
            (*this)[size() - 1].bondNieghbor(2, (*this)[size() - 1]);
        }

    }
    (*this)[size() - 1].bondNieghbor(2, (*this)[size() - edges.size()]);
    (*this)[size() - edges.size()].bondNieghbor(1, (*this)[size() - 1]);
}

void ConvexHull::setHorizon(vector<Edge>& horizon, SmartPoint& star) {

    Edge search(0, star.facingFacets[0]);

    while (search.outside()->faces(star)) {
        search.flip();
        search.changeDir();
        search.progress();
    }

    horizon.push_back(Edge(search));

    for (search.progress(); !(search == horizon[0]); search.progress()) {
        while (search.outside()->faces(star)) {
            search.flip();
            search.progress();
        }
        horizon.push_back(Edge(search));
    }
}

void ConvexHull::removeFace(SmartPoint& star) {
    for (int i = 0; i < star.facingFacets.size(); i++) {
        (*star.facingFacets[i]).disable();
    }
}

void ConvexHull::updateConflifctGraph(vector<Edge>& horizon, SmartPoint& star) {
    vector<SmartPoint> starbin;
    for (int i = 0; i < star.facingFacets.size(); i++) {
        for (int j = 0; j < (*star.facingFacets[i]).size(); j++) {
            starbin.push_back((*star.facingFacets[i])[j]);
        }
        (*star.facingFacets[i]).decouple();
    }
    for (int i = 0; i < horizon.size(); i++) {
        for (int j = 0; j < horizon[i].outside()->size(); j++) {
            starbin.push_back((*horizon[i].outside())[j]);
        }
    }

    for (int i = 0; i < horizon.size(); i++)
        for (int j = 0; j < starbin.size(); j++)
            if ((*this)[size() - i].faces(starbin[j])) {
                starbin[j].bondFacet((*this)[size() - i]);
            }
}

void ConvexHull::removeDisabledFacets() {
    int shift = 0;
    for (int i = 0; i < size() - shift; i++) {
        if (!(*this)[i].enabled()) shift++;
        (*this)[i] = (*this)[i + shift];
    }
    for (int i = 0; i < shift; i++) pop_back();

}

ostream& operator<<(ostream& os, ConvexHull ch) {
    for (int i = 0; i < ch.size(); i++)
        os << ch[i] << endl;
    return os;
}