
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
#define self (*this)

/**
 * The constructor.
 * @param chLeft A convex hull with all of its elements entirely to the left of
 * all the elements in the right convex hull.
 * @param chRight a convex hull with all of its elements entirely to the right 
 * of the left convex hull.
 */
GiftWrapping::GiftWrapping(ConvexHull& chLeft, ConvexHull& chRight) {

    assert(!chLeft.empty());
    assert(!chRight.empty());
    assert(chLeft.testNeighbors() && chRight.testNeighbors());
    assert(chLeft.isConvex(false));
    assert(chRight.isConvex(false));

    reserve(10 + chLeft.size()*3 + chRight.size()*3);

    buildBridge(chLeft, chRight);
    bondFlatNeighbors();

    gut(*leftInside);
    gut(*rightInside);

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

    HullProj2d hpLeft(chLeft);
    HullProj2d hpRight(chRight);

    supportingLine(hpLeft, leftSupEdge, hpRight, rightSupEdge);

    int leftFacet, leftInd, rightFacet, rightInd;
    chLeft.locPoint(hpLeft[leftSupEdge], leftFacet, leftInd);
    chRight.locPoint(hpRight[rightSupEdge], rightFacet, rightInd);

    left.inside = &chLeft[leftFacet];
    left.setIndex(leftInd);
    right.inside = &chRight[rightFacet];
    right.setIndex(rightInd);

    nextHorizon(left, right.a(), COUNTER);
    leftInside = left.inside;
    nextHorizon(right, left.a(), CLOCKWISE);
    rightInside = right.outside();
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

    push_back(SmartFacet(leftEdge.a(), leftEdge.b(), rightEdge.a()));
    self[0].setNeighbor(0, leftEdge.outside());
    leftEdge.outside()->setNeighbor(leftEdge.outsideIndex(), &self[0]);

    stl();

    for (int i = 0; !endBridge(); i++) {
        if (i % 2 == 0) addFacet(rightEdge, CLOCKWISE, leftEdge);
        else addFacet(leftEdge, COUNTER, rightEdge);
    }

    neighborLastToFirst();

}

/**
 * Sets the last bridge facet to be neighbors with the first bridge facet. 
 * @param clockwiseLast was the facet added in the clockwise or 
 * counter clockwise direction?
 */
void GiftWrapping::neighborLastToFirst() {

    self[0].setNeighbor(self[0].counterCl() ? 2 : 1, &self[size() - 1]);
    self[size() - 1].setNeighbor(self[size() - 1].counterCl() ? 1 : 2, &self[0]);
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

    if (!nextHorizon(edge, farEdge.b(), counter)) {
        farEdge.progress();
        return;
    }

    if (size() > 0 && self[size() - 1].faces(edge.b())) {
        removeLast(farEdge);
        farEdge.backUp();
    }

    if (counter)push_back(SmartFacet(edge.a(), edge.b(), farEdge.b()));
    else push_back(SmartFacet(edge.b(), edge.a(), farEdge.b()));

    stl();
    setNeighbors(edge);
    farEdge.progress();

}

/**
 * Remove bad neighboring created by  the last facet added to the wrong side.
 * @param edge the edge at the base of the facet removed.
 */
void GiftWrapping::removeLast(SmartEdge& edge) {

    SmartFacet* out = self[size() - 1].getNeighbor(0);
    int outIndex = out->neighborIndex(&self[size() - 1]);
    out->setNeighbor(outIndex, self[size() - 1].counterCl() ? edge.inside : edge.oldOutside);

    pop_back();

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
void GiftWrapping::setNeighbors(SmartEdge& edge) {

    bool counterClock = (*this)[size() - 1].counterCl();

    SmartFacet* night = counterClock ? edge.outside() : edge.inside;
    int nightIndex = counterClock ? edge.outsideIndex() : edge.getIndex();
    if (!counterClock) edge.oldOutside = edge.outside();

    self[size() - 1].setNeighbor(0, night);
    night->setNeighbor(nightIndex, &self[size() - 1]);

    if (size() > 1) {
        int leadingEdge = self[size() - 2].counterCl() ? 1 : 2;
        int trailingEdge = self[size() - 1].counterCl() ? 2 : 1;
        self[size() - 1].setNeighbor(trailingEdge, &self[size() - 2]);
        self[size() - 2].setNeighbor(leadingEdge, &self[size() - 1]);
    }
}

/**
 * Rotates an edge around it's a() value until it sits on a horizon.
 * @param edge the edge to search for the horizon.
 * @param star the point that horizon vissabuility is related to.  From one side
 * of the horizon the point is vissable and from the other it is not.
 * @param clockwise the direction the horizon edge goes.
 * @return false if the horizon has returned to the beginning.
 */
bool GiftWrapping::nextHorizon(SmartEdge& edge, Point star, bool counter) {
    int i = 0;
    while ((counter ? edge.outside() : edge.inside)->faces(star) || //day 
            !(counter ? edge.inside : edge.outside())->faces(star)) { //night
        assert(i++ < 50);
        edge.flip();
        edge.progress();
        if (edge.outside() == nullptr) return false;

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
void GiftWrapping::supportingLine(HullProj2d& hpLeft, int& lSupEdge, 
                                  HullProj2d& hpRight, int& rSupEdge) {

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
void GiftWrapping::bondFlatNeighbors() {
    vector<SmartFacet*> lefts, rights;
    for (int i = 0; i < size() && (lefts.size() < 3 || rights.size() < 3); i++)
        if (self[i].counterCl()) lefts.push_back(&self[i]);
        else rights.push_back(&self[i]);
    if (lefts.size() == 2) bondBasesForFlatNeigbors(*lefts[0], *lefts[1]);
    if (rights.size() == 2) bondBasesForFlatNeigbors(*rights[0], *rights[1]);
}

/**
 * bonds the bases of these neighbors
 */
void GiftWrapping::bondBasesForFlatNeigbors(SmartFacet& f1, SmartFacet& f2) {
    f1.setNeighbor(0, &f2);
    f2.setNeighbor(0, &f1);
}
