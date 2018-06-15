
/* 
 * File:   GiftWrapping.cpp
 * @author: Dov
 * 
 * Created on May 27, 2018, 4:22 PM
 */

#include "GiftWrapping.h"
#include "HullProj2d.h"
#include "SmartEdge.h"
#include <assert.h>
#include <unordered_set>
#include <omp.h>
#define self (*this)

/**
 * The constructor.
 * @param chLeft A convex hull with all of its elements entirely to the left of
 * all the elements in the right convex hull.
 * @param chRight a convex hull with all of its elements entirely to the right 
 * of the left convex hull.
 */
GiftWrapping::GiftWrapping(ConvexHull& chLeft, ConvexHull& chRight) {
    build(chLeft, chRight);
}

/**
 * Gift wraps this empty CH.
 * @param chLeft A convex hull with all of its elements entirely to the left of
 * all the elements in the right convex hull.
 * @param chRight a convex hull with all of its elements entirely to the right 
 * of the left convex hull.
 */
void GiftWrapping::build(ConvexHull& chLeft, ConvexHull& chRight) {
    assert(!chLeft.empty());
    assert(!chRight.empty());
    assert(chLeft.testNeighbors() && chRight.testNeighbors());
    assert(chLeft.isConvex(false));
    assert(chRight.isConvex(false));

    reserve(10 + chLeft.size()*3 + chRight.size()*3);

    buildBridge(chLeft, chRight);

    assert(isConvex(false));
    setNeighbors();

    omp_set_num_threads(2);
#pragma omp parallel
    {
        bool fewThreads = omp_get_num_threads() < 2;
        if (omp_get_thread_num() == 0 || fewThreads) gut(*leftInside);
        if (omp_get_thread_num() == 1 || fewThreads) gut(*rightInside);
    }

    importFacets(chLeft);
    importFacets(chRight);
    setHullPoints();
    assert(isConvex(false));

}

/**
 * eliminates all the facets from one hull on the inside of the bridge.
 * @param start one facet inside the bridge.
 */
void GiftWrapping::gut(SmartFacet& start) {
    start.disable();
    for (int i = 0; i < start.size(); i++) {
        if (start.getNeighbor(i)->enabled() &&
                start.getNeighbor(i)->neighborIndex(&start) != -1) {
            gut(*start.getNeighbor(i));
        }
    }
}

/**
 * initializes the edges that will track the horizons to start at the peaks. 
 * @param left this edge will be assigned values to start it in a counter 
 * clockwise direction around the left hulls horizon.
 * @param right this edge will be assigned values to start it in a 
 * clockwise direction around the right hulls horizon.
 * @param chLeft the left hull
 * @param chRight the right hull
 */
void GiftWrapping::initEdges(SmartEdge& left, SmartEdge& right,
        ConvexHull& chLeft, ConvexHull& chRight) {
    int leftSupEdge, rightSupEdge;

    HullProj2d hpLeft, hpRight;
    buildHullProjections(hpLeft, hpRight, chLeft, chRight);

    supportingLine(hpLeft, leftSupEdge, hpRight, rightSupEdge);

    int leftFacet, leftInd, rightFacet, rightInd;

#pragma omp parallel num_threads(2)
    {
        int id = omp_get_thread_num();
        bool enoughThreads = omp_get_num_threads() >= 2;
        if (id == 0 || !enoughThreads) {
            chLeft.locPoint(hpLeft[leftSupEdge], leftFacet, leftInd);
            left.setInside(&chLeft[leftFacet]);
            left.setIndex(leftInd);
        }
        if (id == 1 || !enoughThreads) {
            chRight.locPoint(hpRight[rightSupEdge], rightFacet, rightInd);
            right.setInside(&chRight[rightFacet]);
            right.setIndex(rightInd);
        }
    }
#pragma omp parallel num_threads(2)
    {
        int id = omp_get_thread_num();
        bool enoughThreads = omp_get_num_threads() >= 2;
        if (id == 0 || !enoughThreads) {
            nextHorizon(left, right.a(), COUNTER);
            leftInside = left.getInside();
        }
        if (id == 1 || !enoughThreads) {
            nextHorizon(right, left.a(), CLOCKWISE);
            rightInside = right.getOutside();
        }
    }
}

/**
 * builds the 2d projections of each of the convex hulls.
 * @param hpLeft the left projection
 * @param hpRight the right projection
 */
void GiftWrapping::buildHullProjections(HullProj2d& hpLeft, HullProj2d& hpRight,
        ConvexHull& chLeft, ConvexHull& chRight) {
    omp_set_num_threads(2);
#pragma omp parallel
    {
        int id = omp_get_thread_num();
        bool enoughThreads = omp_get_num_threads() >= 2;
        if (id == 0 || !enoughThreads)hpLeft.build(chLeft);
        if (id == 1 || !enoughThreads)hpRight.build(chRight);
    }
}

/**
 * Constructs the bridge between the two horizons.
 * @param chLeft the left convex hull
 * @param chRight the right convex hull
 * @param insideLeft Will be assigned an arbitrary facet address on the inside
 * of the left hull
 * @param insideRight will be assigned and arbitrary facet value on the inside 
 * of the right hull.
 */
void GiftWrapping::buildBridge(ConvexHull& chLeft, ConvexHull& chRight) {

    SmartEdge leftEdge(-1, nullptr), rightEdge(-1, nullptr);
    initEdges(leftEdge, rightEdge, chLeft, chRight);

    for (int i = !firstFacet(leftEdge, rightEdge); !endBridge(); i++) {
        if (i % 2 == 0) addFacet(rightEdge, CLOCKWISE, leftEdge);
        else addFacet(leftEdge, COUNTER, rightEdge);
    }

}

/**
 * adds the first facet
 * @param leftEdge the left edge traversing the horizon
 * @param rightEdge the right edge traversing the horizon
 * @return true if the first facet is on the left side, false if it's on the 
 * right.
 */
bool GiftWrapping::firstFacet(SmartEdge& leftEdge, SmartEdge& rightEdge) {
    push_back(SmartFacet(leftEdge.a(), leftEdge.b(), rightEdge.a()));
    self[0].setNeighbor(0, leftEdge.getOutside());

    if (self[0].faces(rightEdge.b()) || self[0].faces(rightEdge.a())) {
        pop_back();
        push_back(SmartFacet(rightEdge.b(), rightEdge.a(), leftEdge.a()));
        self[0].setNeighbor(0, rightEdge.getInside());

        return 0;
    }
    return 1;
}

/**
 * adds a facet to the bridge and makes sure it has the right neighbors.
 * @param edge the edge at the base of the facet
 * @param star the edge at the pouint of the facet on the far hull from the base
 * edge
 * @param clockWise is this horizon progressing clockwise or counter clockwise.
 * The left hull is counter and the right is clock.
 * @return true if the facet was added without hickups, false if the previous 
 * facet needed to be removed to add the new one.  False is returned when it's
 * nescesary to add multiple facets on the same side instead of switching off.
 */
void GiftWrapping::addFacet(SmartEdge& edge, bool counter, SmartEdge& farEdge) {

    SmartEdge checkA(edge);
    nextHorizon(checkA, farEdge.a(), counter);

    if (!nextHorizon(edge, farEdge.b(), counter) || (size() > 0 &&
            (self[size() - 1].faces(edge.b()) ||
            self[size() - 1].faces(checkA.b())))) {
        edge = checkA;
        pop_back();
        farEdge.backUp();

    }

    if (counter) {
        push_back(SmartFacet(edge.a(), edge.b(), farEdge.b()));
        self[size() - 1].setNeighbor(0, edge.getOutside());
    } else {
        push_back(SmartFacet(edge.b(), edge.a(), farEdge.b()));
        self[size() - 1].setNeighbor(0, edge.getInside());
    }

    farEdge.progress();

}

/**
 * the condition for ending the bridge
 * @return 
 */
bool GiftWrapping::endBridge() {
    if (size() < 3) return false;

    SmartEdge front((self[size() - 1].counterCl() ? 1 : 2), &self[size() - 1]),
            back((self[0].counterCl() ? 2 : 1), &self[0]);

    return front == back;
}

/**
 * Sets up the neighbors for the most recently added facet.
 * @param edge the base of the facet
 * @param counterClock the direction the facet moves around the horizon.
 */
void GiftWrapping::setNeighbors() {

    for (int i = 0; i < size(); i++) {
        int neighborInd = self[i].getNeighbor(0)->pointIndex(self[i][1]);
        self[i].getNeighbor(0)->setNeighbor(neighborInd, &self[i]);
        if (self[i].counterCl()) {
            self[i].setNeighbor(1, &self[(i + 1) % size()]);
            self[i].setNeighbor(2, &self[i > 0 ? (i - 1) : size() - 1]);
        } else {
            self[i].setNeighbor(2, &self[(i + 1) % size()]);
            self[i].setNeighbor(1, &self[i > 0 ? (i - 1) : size() - 1]);
        }
    }
    bondBaseNeighbors();
}

/**
 * Rotates an edge around it's a() value until it sits on a horizon.
 * @param edge the edge to search for the horizon.
 * @param star the point that horizon vissabuility is related to.  From one side
 * of the horizon the point is vissable and from the other it is not.
 * @param clockwise the direction the horizon edge goes.
 * @return false if the horizon has returned to the beginning.
 */
bool GiftWrapping::nextHorizon(SmartEdge& edge, const Point& star, bool counter) const {
    SmartEdge start(edge);
    while ((counter ? edge.getOutside() : edge.getInside())->faces(star) || //day 
            !(counter ? edge.getInside() : edge.getOutside())->faces(star)) { //night
        edge.flip();
        edge.progress();
        if (start == edge) return false;

    }
    return true;
}

/**
 * Finds the supporting line connecting the top of the two hulls.  If a beam was 
 * dropped, these two points would support it.
 * @param hpLeft a 2d projection of the left hull.
 * @param lSupEdge to be assigned the index on the projection of the support
 *  point.
 * @param hpRight a 2d projection of the right hull.
 * @param rSupEdge Like the lSupEdge, but for the right 2d proj.
 */
void GiftWrapping::supportingLine(const HullProj2d& hpLeft, int& lSupEdge,
        const HullProj2d& hpRight, int& rSupEdge) const {

    lSupEdge = hpLeft.maxX();
    rSupEdge = hpRight.minX();

    int oldLeftInd, oldRightInd;
    do {

        oldLeftInd = lSupEdge;
        oldRightInd = rSupEdge;

        while (hpLeft.edgeFaces(lSupEdge, hpRight[rSupEdge])) {
            lSupEdge = (lSupEdge + 1) % hpLeft.size();

        }
        while (hpRight.edgeFaces(rSupEdge - 1, hpLeft[oldLeftInd])) {
            rSupEdge--;
            if (rSupEdge < 0) rSupEdge = hpRight.size() + rSupEdge;
        }
    } while (oldLeftInd != lSupEdge || oldRightInd != rSupEdge);
}

/**
 * checks to see if two elements of the bridge should be neighbors at their 
 * bases (0 index).
 */
void GiftWrapping::bondBaseNeighbors() {

    //  TODO:  This needs to be made to work.

    struct EdgeHasher{
    public:
        size_t operator()(const SmartEdge& edge) const {
            Point a(edge.a());
            Point b(edge.b());

            uint32_t h1 = std::hash<double>()(a.x);
            uint32_t h2 = std::hash<double>()(a.y);
            size_t h3 = std::hash<double>()(a.z);

            size_t h4 = std::hash<double>()(b.x);
            size_t h5 = std::hash<double>()(b.y);
            size_t h6 = std::hash<double>()(b.z);

            size_t result1 = (h1 ^ (h2 << 1)) ^ h3;
            size_t result2 = (h4 ^ (h5 << 6)) ^ h3;

            return result1 + result2;
        }
    };

    struct EdgeComparator {

        bool operator()(const SmartEdge& e1, const SmartEdge& e2) const {
            return e1.a().distSquared(e2.b()) < 1e-5  && e1.b().distSquared(e2.a()) < 1e-5;
        }
    };

    unordered_set<SmartEdge, EdgeHasher, EdgeComparator> edgeSet;
    for (int i = 0; i < size(); i++) {
        SmartEdge baseI(0, &self[i]);
        if (!(edgeSet.insert(baseI).second)) {
            bondBasesForFlatNeighbors(&self[i], (*edgeSet.find(baseI)).getOutside());
        }
    }
    //The code below, when in place of the code above, works.  It's just too slow.
    //    for (int i = 0; i < size(); i++)
    //        for (int j = i + 1; j < size(); j++)
    //            if (self[i][0] == self[j][1] && self[i][1] == self[j][0])
    //                bondBasesForFlatNeighbors(self[i], self[j]);

}

/**
 * bonds the bases of these neighbors
 */
void GiftWrapping::bondBasesForFlatNeighbors(SmartFacet* f1, SmartFacet* f2) {
    f1->setNeighbor(0, f2);
    f2->setNeighbor(0, f1);
}
