/**
 * @author Dov Neimand
 */

#include <unordered_set>

#include "ConvexHull.h"
#include "SmartPoint.h"
#include "Edge.h"

using namespace std;

/**
 * The constructor.
 * @param points The points to be encompassed by this convex hull.
 */
ConvexHull::ConvexHull(vector<SmartPoint>& points) {
    reserve(2 * points.size());
    int pointIndex = firstFacet(points);

    pyramid(points[pointIndex++]);

    for (int i = 0; i < size(); i++)
        (*this)[i].initPoints(points, pointIndex);

    for (; pointIndex < points.size(); pointIndex++) {

        if (points[pointIndex].inside()) continue;

        vector<Edge> horizon;
        setHorizon(horizon, points[pointIndex]);

        cone(points[pointIndex], horizon);
        updateConflifctGraph(horizon, points[pointIndex]);
    }
    removeDisabledFacets();
}

/**
 * Sets up the first facet and adds it to this. This facet is selected from
 * the first points.
 * @param points The points the convex hull is meant to encompass and the first
 * facet taken from.
 * @return the index of the first point not in the first facet.
 */
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

    //makes a polygon if necessary
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

/**
 * Makes the first pyramid and adds the facets to this.
 * @param tip the point at the top of the pyramid.
 */
void ConvexHull::pyramid(SmartPoint& tip) {

    if ((*this)[0].faces(tip)) (*this)[0].flip();

    for (int j = 0; j < (*this)[0].size(); j++) {

        push_back(SmartFacet(3));

        (*this)[size() - 1].push_back((*this)[0][(j + 1) % (*this)[0].size()]);
        (*this)[size() - 1].push_back((*this)[0][j]);
        (*this)[size() - 1].push_back(tip);

        (*this)[0].setNieghbor(j, (*this)[size() - 1]);
        (*this)[size() - 1].setNieghbor(0, (*this)[0]);
        if (j > 0) {
            (*this)[size() - 1].setNieghbor(1, (*this)[size() - 2]);
            (*this)[size() - 2].setNieghbor(2, (*this)[size() - 1]);
        }
    }
    (*this)[size() - 1].setNieghbor(2, (*this)[size() - (*this)[0].size()]);
    (*this)[size() - (*this)[0].size()].setNieghbor(1, (*this)[size() - 1]);

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
void ConvexHull::cone(SmartPoint& tip, std::vector<Edge>& edges) {

    for (int i = 0; i < edges.size(); i++) {

        push_back(SmartFacet(3));
        edges[i].replacementInside = &((*this)[size() - 1]);
        (*this)[size() - 1].push_back(edges[i].a());
        (*this)[size() - 1].push_back(edges[i].b());
        (*this)[size() - 1].push_back(tip);

        edges[i].outside()->setNieghbor(edges[i].outsideIndex(), (*this)[size() - 1]); //if no mistake elsewhere check outside index
        (*this)[size() - 1].setNieghbor(0, *(edges[i].outside()));
        if (i > 0) {
            (*this)[size() - 1].setNieghbor(2, (*this)[size() - 2]);
            (*this)[size() - 2].setNieghbor(1, (*this)[size() - 1]);
        }

    }
    (*this)[size() - 1].setNieghbor(1, (*this)[size() - edges.size()]);
    (*this)[size() - edges.size()].setNieghbor(2, (*this)[size() - 1]);
}

/**
 * Builds a vector of edges on the convex hull that describe the edge of 
 * visibility from a given point.
 * @param horizon Should come in empty and finish with a list of edges 
 * constituting the edge of visability from the star.
 * @param star the point from which those edges make up the horizon.
 * 
 */
void ConvexHull::setHorizon(vector<Edge>& horizon, SmartPoint& star) {

    Edge search(0, star.facingFacets[0]);

    while (search.outside()->faces(star)) {
        search.flip();
        search.changeDir();
        search.progress();
    }

    horizon.push_back(Edge(search));

    for (search.progress(); !(horizon[0] == search); search.progress()) {
        if (search.outside()->faces(star)) search.flip();
        else horizon.push_back(Edge(search));
    }
}

/**
 * Updates the conflict graph.  
 * @param horizon the most recent horizon used to add a cone.
 * @param departingStar the point at the tip of the cone most recently added.
 */
void ConvexHull::updateConflifctGraph(vector<Edge>& horizon, SmartPoint& departingStar) {

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

/**
 * removes from this all facets marked as garbage.
 */
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

/**
 * A printable list of the facets in ch
 * @param os an ostream to output the convex hull desacription
 * @param ch the convex hull to be printed.
 * @return the ostream.
 */
ostream& operator<<(ostream& os, ConvexHull ch) {
    for (int i = 0; i < ch.size(); i++) {
        //        if (!ch[i].enabled()) continue;
        os << ch[i] << endl;
    }
    return os;
}