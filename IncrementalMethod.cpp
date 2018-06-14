/**
 * @author Dov Neimand
 */
#include "IncrementalMethod.h"
#include <unordered_set>
#include<assert.h>
#include "SmartPoint.h"
#include "SmartEdge.h"
#include <cmath>
#include <omp.h>

#define self (*this)

using namespace std;

/**
 * The constructor.
 * @param points The points to be encompassed by this convex hull.
 */
IncrementalMethod::IncrementalMethod(vector<SmartPoint>& points) {
    if (points.size() == 0) return;

    reserve(2 * points.size() * points.size());
    int pointIndex = findFirstFacet(points, 0, true);

    pyramid(points[pointIndex++]);
    assert(isConvex(false));
    initConflictGraph(points, pointIndex);

    for (; pointIndex < points.size(); pointIndex++) {
        if (points[pointIndex].inside()) continue;

        vector<SmartEdge> horizon;
        setHorizon(horizon, points[pointIndex]);
        cone(points[pointIndex], horizon);
        updateConflifctGraph(horizon, points[pointIndex]);
        assert(isConvex(false));
        assert(testNeighbors());

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
    assert(testNeighbors());

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

        edges[i].setReplacementInside(&(self[size() - 1]));

        edges[i].getOutside()->setNeighbor(edges[i].outsideIndex(), &self[size() - 1]); //if no mistake elsewhere check outside index
        self[size() - 1].setNeighbor(0, edges[i].getOutside());
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
void IncrementalMethod::setHorizon(vector<SmartEdge>& horizon, SmartPoint& star) {

    SmartEdge search(0, star.getFaceingFacet(0));
    findHorizon(search, star);

    horizon.push_back(search);
    for (search.progress(); !(horizon[0] == search); search.progress()) {
        if (search.getOutside()->faces(star)) search.flip();
        else horizon.push_back(SmartEdge(search));
    }
}

/**
 * initializes the conflict graph
 * @param points the points that will store the facets they face
 * @param pointIndex the index of the last point added to the CH
 */
void IncrementalMethod::initConflictGraph(vector<SmartPoint>& points, int pointIndex) {

#pragma omp parallel
    {
        int id = omp_get_thread_num();
        int numThreads = omp_get_num_threads();
        for (int i = id; i < points.size(); i += numThreads)
            points[i].reserveFacingFacetSpace(size()*2);
    }

#pragma omp parallel
    {
        int id = omp_get_thread_num();
        int numThreads = omp_get_num_threads();
        for (int i = id; i < size(); i += numThreads)
            self[i].initPoints(points, pointIndex);

    }

}

/**
 * finds the horizon from any day edge
 * @param edge an edge with an inside facet facing the star
 * @param star the point the horizon is visable from

 */
void IncrementalMethod::findHorizon(SmartEdge& edge, const Point& star) {

    SmartEdge bestPath(edge);
    SmartEdge start(edge);
    while (edge.getOutside()->faces(star)) {
        edge.flip();
        edge.progress();
        if (edge.b().d(star) > bestPath.b().d(star)) bestPath = edge;
        if (edge == start) {
            start = bestPath;
            edge = start;
            edge.progress();
        }
    }
}

/**
 * Updates the conflict graph.  
 * @param horizon the most recent horizon used to add a cone.
 * @param departingStar the point at the tip of the cone most recently added.
 */
void IncrementalMethod::updateConflifctGraph(vector<SmartEdge>& horizon, SmartPoint& departingStar) {

    while (departingStar.numFacingFacets() > 0)
        departingStar.getFaceingFacet(departingStar.numFacingFacets() - 1)->disable();


    unordered_set<Point*> duplicateCheck;
    for (int edgeIndex = 0; edgeIndex < horizon.size(); edgeIndex++) {

        SmartFacet* surface = horizon[edgeIndex].getReplacementInside();

        for (int j = 0; j < horizon[edgeIndex].getOutside()->numStars(); j++) {
            SmartPoint* newStar = horizon[edgeIndex].getOutside()->star(j);
            if (surface->faces(*newStar)) {
                surface->bondPoint(*newStar);
                duplicateCheck.insert(newStar);
            }

        }

        for (int j = 0; j < horizon[edgeIndex].getInside()->numStars(); j++) {
            SmartPoint* newStar = horizon[edgeIndex].getInside()->star(j);
            if (surface->faces(*newStar)
                    && duplicateCheck.insert(newStar).second)
                surface->bondPoint(*newStar);
        }

        duplicateCheck.clear();
    }
}

