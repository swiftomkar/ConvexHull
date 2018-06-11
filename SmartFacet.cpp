/**
 * @author Dov Neimand
 */
#include "SmartFacet.h"
#include "SmartPoint.h"
#include <algorithm>
#include <cmath>

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
void SmartFacet::setNeighbor(int side, SmartFacet* f) {
    while (side >= neighbors.size())
        neighbors.push_back(nullptr);
    neighbors[side] = f;
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
bool SmartFacet::enabled() const {
    return isEnabled;
}

/**
 * returns the neighbor at index i, the edge the neighbor is up against.
 * @param i the index of the desired neighbor.
 * @return the neighbor at the given index.
 */
SmartFacet* SmartFacet::getNeighbor(int i) {
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
 * Constructor, points should be ordered counter clockwise facing out.
 * @param a a point in the new facet
 * @param b a point in the new facet
 * @param c a point in the new facet
 */
SmartFacet::SmartFacet(const Point& a, const Point& b, const Point& c) : SmartFacet(3) {
    push_back(a);
    push_back(b);
    push_back(c);
}

/**
 * Moves all the pointers of the neighbors to their new addresses.  
 * TODO: Their might be a bug here if different neighbors get shifted different
 * amounts.  Seems to work for now.
 * 
 * @param shift the distance to shift the neighbor addresses to correspond to
 * their new homes.
 */
void SmartFacet::changeOfAddress(int shift) {
    changeOfAddress(this -shift);
}

void SmartFacet::changeOfAddress(SmartFacet* toAddress) {

    for (int i = 0; i < size(); i++) {

        int neighborIndex = neighbors[i]->neighborIndex(this);
        neighbors[i]->neighbors[neighborIndex] = toAddress;

    }
}

/**
 * Is f a neighbor?
 * @param f the neighbor to search for.
 * @return -1 if f is not neighbor, otherwise the index of the neighbor.
 */
int SmartFacet::neighborIndex(const SmartFacet* f) {

    for (int i = 0; i < size(); i++) if (neighbors[i] == f) return i;
    return -1;

}

/**
 * Are the neighbors set up properly.
 * @return true if they are and false if they are not.
 */
bool SmartFacet::testNeighbors() {

    if (!enabled()) {
        cerr << "bad facet tested" << endl;
        return false;
    }
    if (neighbors.size() != size()) {
        cerr << "neighbors size != size" << endl;
        return false;
    }
    for(int i = 0; i < size(); i++)
        for(int j = i+1; j < size(); j++)
            if(neighbors[i] == neighbors[j]){
                cout << (*this) << " has the same neighbor " << *neighbors[i] << "at both indeces " << *neighbors[j] << endl;
                return false;
            }
            
    for (int i = 0; i < size(); i++) {
        int neighborInd = neighbors[i]->neighborIndex(this);
        if (neighborInd == -1) {
            cout << *this << " is not properly connected with " << *neighbors[i] << " at index " << i << endl;
            return false;
        }
        return true;
    }
}

/**
 * returns the index of a point and -1 if the point is not found.
 * @param p
 * @return 
 */
int SmartFacet::pointIndex(Point p) {
    for (int i = 0; i < size(); i++) if ((*this)[i] == p) return i;
    return -1;
}

/**
 * appends vector and neigbor information to a vector of doubles for 
 * mpi transport.  Each neighbor is written as neighbor address - this address.
 * immediately following point information.  The Facet ends with NAN.  For
 *  example:  x1, y1, z1, n1, x2, y2, z2, n2, x3, y3, z3, n2, NAN
 * 
 * @param vec the vector appended to.
 */
void SmartFacet::appendToVector(std::vector<double>& vec) {
    for (int i = 0; i < size(); i++){
        (*this)[i].writeToVector(vec);
        vec.push_back(neighbors[i] - this);
    }
    vec.push_back(NAN);
}

/**
 * Constructor
 * Loads a smart facet from an array of doubles.  Inverse of append to vector.
 * @param vec the vector to load from
 * @param vecIndex the index of the vector where the first element of this
 * facet can be found.
 * @param address the address this facet will be stored at in its convex hull.
 */
SmartFacet::SmartFacet(const std::vector<double>& vec, int& vecIndex, SmartFacet* address) {
    reserve(3);
    while (vec[vecIndex] == vec[vecIndex]) {//is not NAN
        push_back(SmartPoint(vec, vecIndex));
        vecIndex += 3;
        neighbors.push_back(address + (int)vec[vecIndex]);
        vecIndex++;
    }
}

/**
 * finds the first neighbor index that is empty.  
 * @return the first neighbor index that's empty.  If none exists, returns -1;
 */
int SmartFacet::emptyNeighbor() const {
    cout << "empty neighbor seaerch " << endl;
    for(int i = 0; i < neighbors.size(); i++){
        cout << "at " << i << " lives " << *neighbors[i] << endl;
        if(neighbors[i] == nullptr) return i;
    }
    if (neighbors.size() < size()) {
        cout << "missing neighbors, returning" << neighbors.size() << endl;
        return neighbors.size();
    }
    return -1;
}

/**
 * Is the x value of the third point greater than that of the first two.
 * When used with gift wrapping, this will tell if the facet is based out of the
 * left CH.
 * @return true if the facet is based off a left CH and false if right.
 */
bool SmartFacet::counterCl() const {
    if((*this)[2].x >= (*this)[0].x && (*this)[2].x >= (*this)[1].x) return true;
    return false;
}
