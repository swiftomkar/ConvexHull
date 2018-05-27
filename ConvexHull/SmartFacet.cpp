/**
 * @author Dov Neimand
 */
#include "SmartFacet.h"
#include "SmartPoint.h"
#include "Edge.h"

/**
 * bonds a point to this facet and this facet to the given point
 * @param p the point to be bound.
 */
void SmartFacet::bondPoint(SmartPoint& p) {
    sky.push_back(&p);
    p.facingFacets.push_back(this);
}

/**
 * finds all the points in the given list that face this facet and stores them.
 * @param points the points to be searched.
 * @param start
 */
void SmartFacet::initPoints(std::vector<SmartPoint>& points, int start) {
    for (int i = start; i < points.size(); i++)
        if (faces(points[i])) bondPoint(points[i]);
}

/**
 * sets the given facet as a neighbor at the provided index
 * @param side the index of the new neighbor
 * @param f the neighbor being added
 */
void SmartFacet::setNieghbor(int side, SmartFacet & f) {
    while (side >= neighbors.size())
        neighbors.push_back(nullptr);
    neighbors[side] = &f;
}

/**
 * marks this facet as disabled and breaks the link with all the starts facing 
 * this.
 */
void SmartFacet::disable() {
    isEnabled = false;
    for (int i = 0; i < sky.size(); i++) {
        int shift = 0;

        for (int j = 0; j + shift < sky[i]->facingFacets.size(); j++) {
            if (sky[i]->facingFacets[j + shift] == this) {
                shift++;
                j--;
                continue;
            }
            sky[i]->facingFacets[j] = sky[i]->facingFacets[j + shift];
        }
        for (int j = 0; j < shift; j++)
            sky[i]->facingFacets.pop_back();
    }
}

/**
 * Is the facet enabled
 * @return true if it's enabled, false otherwise.
 */
bool SmartFacet::enabled() {
    return isEnabled;
}

/**
 * returns the neighbor at index i, the edge the neighbor is up against.
 * @param i the index of the desired neighbor.
 * @return the neighbor at the given index.
 */
SmartFacet* SmartFacet::getNeighbor(int i){
    return neighbors[i];
}

/**
 * The constructor
 * @param expectedSize reserves space in memory.
 */
SmartFacet::SmartFacet(int expectedSize) {
    reserve(expectedSize);
}

/**
 * Checks to see if all the neighbors are properly placed. 
 * @return true if all the neighbors are in the correct positions.
 */
bool SmartFacet::verifyNeighbors(){
    for (int i = 0; i < size(); i++) 
        if(!verifyNeighbor(i)) return false;
    return true;
}

/**
 * checks to see if the given neighbor belongs at the given index
 * @param i the index of the neighbor to check and the index of the side of that
 * neighbor.
 * @return true if the neighbor with index i belongs at edge i
 */
bool SmartFacet::verifyNeighbor(int i) {

    Edge e(i, this);
    cout << "verifying " << e << endl;
    for (int j = 0; j < neighbors[i]->size(); j++) {
        if (e == Edge(j, neighbors[i])){
            cout << e << " = " << Edge(j, neighbors[i]) << endl;
            return true;
        } else cout << e << " != " << Edge(j, neighbors[i]) << endl;
    }
    return false;
}
