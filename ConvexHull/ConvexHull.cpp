

#include "ConvexHull.h"
#include "SmartPoint.h"
#include "Edge.h"

using namespace std;

ConvexHull::ConvexHull(vector<SmartPoint>& points) {
    reserve(2 * points.size());
    int pointIndex = firstFacet(points);

    pyramid(points[pointIndex++]);

    for (int i = 0; i < size(); i++)
        (*this)[i].initPoints(points, pointIndex);

    for (int i = 0; i < points.size(); i++) {
        cout << "bonded to " << points[i] << endl;
        for (int j = 0; j < points[i].facingFacets.size(); j++)
            cout << *(points[i].facingFacets[j]) << endl;
    }
    cout << endl;

    for (; pointIndex < points.size(); pointIndex++) {

        cout << endl << "status:" << endl;
        for (int i = 0; i < size(); i++) {
            if (!(*this)[i].enabled()) continue;
            cout << (*this)[i] << " neighbors: " << endl;
            for (int j = 0; j < (*this)[i].size(); j++)
                cout << *(*this)[i].getNeighbor(j) << endl;
            cout << endl;
        }

        if (points[pointIndex].inside()) continue;

        vector<Edge> horizon;
        setHorizon(horizon, points[pointIndex]);

        cone(points[pointIndex], horizon);
        updateConflifctGraph(horizon, points[pointIndex]);
    }
    removeDisabledFacets();
}

int ConvexHull::firstFacet(vector<SmartPoint> points) {
    SmartFacet facet1(5);
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
        ;
        push_back(SmartFacet(3));

        (*this)[size() - 1].push_back((*this)[0][(j + 1) % (*this)[0].size()]);
        (*this)[size() - 1].push_back((*this)[0][j]);
        (*this)[size() - 1].push_back(tip);

        (*this)[0].bondNieghbor(j, (*this)[size() - 1]);
        (*this)[size() - 1].bondNieghbor(0, (*this)[0]);
        if (j > 0) {
            (*this)[size() - 1].bondNieghbor(1, (*this)[size() - 2]);
            (*this)[size() - 2].bondNieghbor(2, (*this)[size() - 1]);
        }
    }
    (*this)[size() - 1].bondNieghbor(2, (*this)[size() - (*this)[0].size()]);
    (*this)[size() - (*this)[0].size()].bondNieghbor(1, (*this)[size() - 1]);

}

void ConvexHull::cone(SmartPoint& tip, std::vector<Edge>& edges) {

    for (int j = 0; j < edges.size(); j++) {

        push_back(SmartFacet(3));
        edges[j].replacementInside = &((*this)[size() - 1]);

        (*this)[size() - 1].push_back(edges[j].a());
        (*this)[size() - 1].push_back(edges[j].b());
        (*this)[size() - 1].push_back(tip);

        edges[j].outside()->bondNieghbor(edges[j].outsideIndex(), (*this)[size() - 1]);//if no mistake elsewhere check outside index
        (*this)[size() - 1].bondNieghbor(0, *(edges[j].outside()));
        if (j > 0) {
            (*this)[size() - 1].bondNieghbor(2, (*this)[size() - 2]);
            (*this)[size() - 2].bondNieghbor(1, (*this)[size() - 1]);
        }

    }
    (*this)[size() - 1].bondNieghbor(1, (*this)[size() - edges.size()]);
    (*this)[size() - edges.size()].bondNieghbor(2, (*this)[size() - 1]);
}

void ConvexHull::setHorizon(vector<Edge>& horizon, SmartPoint& star) {

    Edge search(0, star.facingFacets[0]);

    while (search.outside()->faces(star)) {
        search.flip();
        search.changeDir();
        search.progress();
    }

    horizon.push_back(Edge(search));

    for (search.progress(); !(horizon[0] == search); search.progress()) {
        while (search.outside()->faces(star)) {
            search.flip();
            search.progress();
        }
        if (horizon[0] == search) break;
        horizon.push_back(Edge(search));
    }
}

void ConvexHull::updateConflifctGraph(vector<Edge>& horizon, SmartPoint& departingStar) {

    cout << "bonded to " << departingStar << endl;
    for (int j = 0; j < departingStar.facingFacets.size(); j++)
        cout << *(departingStar.facingFacets[j]) << endl;


    while (departingStar.facingFacets.size() > 0) {
        departingStar.facingFacets[0]->disable();
        cout << " disabling " << *departingStar.facingFacets[0] << " from " << departingStar << endl;
    }

    for (int i = 0; i < horizon.size(); i++) {

        SmartFacet* surface = horizon[i].replacementInside;

        for (int j = 0; j < horizon[i].outside()->sky.size(); j++) {
            SmartPoint* newStar = horizon[i].outside()->sky[j];
            if (surface->faces(*newStar))
                surface->bondPoint(*newStar);

        }

        for (int j = 0; j < horizon[i].inside->sky.size(); j++) {
            SmartPoint* newStar = horizon[i].inside->sky[j];
            if (surface->faces(*newStar))// && !(std::find(surface->begin(), surface->end(), newStar) != surface->end()))//what if it contains newstar already?
                surface->bondPoint(*newStar);
        }
    }


}

void ConvexHull::removeDisabledFacets() {
    int shift = 0;
    for (int i = 0; i < size() - shift; i++) {
        if (!(*this)[i + shift].enabled()) {
            shift++;
            i--;
        } else (*this)[i] = (*this)[i + shift];
    }
    for (int i = 0; i < shift; i++) pop_back();

}

ostream& operator<<(ostream& os, ConvexHull ch) {
    for (int i = 0; i < ch.size(); i++) {
        if (!ch[i].enabled()) continue;
        os << ch[i] << endl;
    }
    return os;
}