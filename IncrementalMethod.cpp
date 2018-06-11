/**
 * @author Dov Neimand
 */
#include "IncrementalMethod.h"
#include <unordered_set>
#include<assert.h>
#include "SmartPoint.h"
#include "SmartEdge.h"
#include <cmath>

#define self (*this)

using namespace std;

/**
 * The constructor.
 * @param points The points to be encompassed by this convex hull.
 */
IncrementalMethod::IncrementalMethod(vector<SmartPoint>& points) {

    reserve(2 * points.size() * points.size());
    int pointIndex = findFirstFacet(points, 0, true);

    pyramid(points[pointIndex++]);

    for (int i = 0; i < size(); i++)
        self[i].initPoints(points, pointIndex);
    
    for (; pointIndex < points.size(); pointIndex++) {

        if (points[pointIndex].inside()) continue;

        vector<SmartEdge> horizon;
        setHorizon(horizon, points[pointIndex]);
        cone(points[pointIndex], horizon);
        updateConflifctGraph(horizon, points[pointIndex]);
    }
    removeDisabledFacets();
    setHullPoints();
    assert(testNeighbors());
    assert(isConvex(false));
}

/**
 * Makes the first pyramid and adds the facets to this.
 * @param tip the point at the top of the pyramid.
 */
void IncrementalMethod::pyramid(const SmartPoint& tip) {

    if (self[0].faces(tip)) self[0].flip();

    for (int j = 0; j < self[0].size(); j++) {

        push_back(SmartFacet(self[0][(j + 1) % self[0].size()], self[0][j], tip));

        self[0].setNeighbor(j, &self[size() - 1]);
        self[size() - 1].setNeighbor(0, &self[0]);
        if (j > 0) {
            self[size() - 1].setNeighbor(1, &self[size() - 2]);
            self[size() - 2].setNeighbor(2, &self[size() - 1]);
        }
    }
    self[size() - 1].setNeighbor(2, &self[1]);
    self[1].setNeighbor(1, &self[size() - 1]);

}

/**
 * Adds a new point to the budding convex hull, and constructs the cone from
 * the previous convex hull to the new point.  The facets of the new cone are 
 * added to this.
 * @param tip the tip of the cone being added.  The new point being appended to
 * the budding convex hull.
 * @param edges The edges on the convex hull at the base of the cone to be added
 * .  These are also the edge of the convex hull that is visable from the point.
 */
void IncrementalMethod::cone(const SmartPoint& tip, std::vector<SmartEdge>& edges) {

    for (int i = 0; i < edges.size(); i++) {
        push_back(SmartFacet(edges[i].a(), edges[i].b(), tip));

        edges[i].replacementInside = &(self[size() - 1]);

        edges[i].outside()->setNeighbor(edges[i].outsideIndex(), &self[size() - 1]); //if no mistake elsewhere check outside index
        self[size() - 1].setNeighbor(0, edges[i].outside());
        if (i > 0) {
            self[size() - 1].setNeighbor(2, &self[size() - 2]);
            self[size() - 2].setNeighbor(1, &self[size() - 1]);
        }
    }

    self[size() - 1].setNeighbor(1, &self[size() - edges.size()]);
    self[size() - edges.size()].setNeighbor(2, &self[size() - 1]);
}

/**
 * Builds a vector of edges on the convex hull that describe the edge of 
 * visibility from a given point.
 * @param horizon Should come in empty and finish with a list of edges 
 * constituting the edge of visability from the star.
 * @param star the point from which those edges make up the horizon.
 * 
 */
void IncrementalMethod::setHorizon(vector<SmartEdge>& horizon, const SmartPoint& star) {

    SmartEdge search(0, star.facingFacets[0]);

    while (search.outside()->faces(star)) {
        search.flip();
        search.changeDir();
        search.progress();
    }

    horizon.push_back(SmartEdge(search));

    for (search.progress(); !(horizon[0] == search); search.progress()) {
        if (search.outside()->faces(star)) search.flip();
        else horizon.push_back(SmartEdge(search));
    }
}

/**
 * Updates the conflict graph.  
 * @param horizon the most recent horizon used to add a cone.
 * @param departingStar the point at the tip of the cone most recently added.
 */
void IncrementalMethod::updateConflifctGraph(const vector<SmartEdge>& horizon, SmartPoint& departingStar) {

    while (departingStar.facingFacets.size() > 0) {
        departingStar.facingFacets[0]->disable();
    }

    unordered_set<Point*> duplicateCheck;
    for (int i = 0; i < horizon.size(); i++) {

        SmartFacet* surface = horizon[i].replacementInside;

        for (int j = 0; j < horizon[i].outside()->sky.size(); j++) {
            SmartPoint* newStar = horizon[i].outside()->sky[j];
            if (surface->faces(*newStar)) {
                surface->bondPoint(*newStar);
                duplicateCheck.insert(newStar);
            }

        }

        for (int j = 0; j < horizon[i].inside->sky.size(); j++) {
            SmartPoint* newStar = horizon[i].inside->sky[j];
            if (surface->faces(*newStar)
                    && duplicateCheck.find(newStar) == duplicateCheck.end())
                surface->bondPoint(*newStar);
        }

        duplicateCheck.clear();
    }
}

