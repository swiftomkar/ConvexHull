/**
 * @author Dov Neimand 
 */
#include <atomic>

#include "SmartPoint.h"
#include "SmartFacet.h"
#include <omp.h>

/**
 * Is this point inside of all the facets it's keeping track of?
 * @return true if it is, false otherwise
 */
bool SmartPoint::inside() const {
    return facingFacets.size() == 0;
}

/**
 * The constructor
 * @param x
 * @param y
 * @param z
 */
SmartPoint::SmartPoint(double x, double y, double z) : Point(x, y, z) {
}

/**
 * creates a smartpoint by extracting values from a specific location in a 
 * vector.  
 * @param vec the vector the values are to be taken from.
 * @param index the index of this point in the vector.
 */
SmartPoint::SmartPoint(const vector<double>& vec, int index)
: Point(vec[index], vec[index + 1], vec[index + 2]) {
}

/**
 * copy constructor
 * @param p
 */
SmartPoint::SmartPoint(Point p) : Point(p) {
}

/**
 * bonds a facet that is facing this point.
 * @param facing the facet to be bonded
 */
void SmartPoint::bondFacet(SmartFacet* facing) {
#pragma omp critical
        facingFacets.push_back(facing);
}

/**
 * returns a pointer to the index i facing facet - a facet that faces this 
 * point.
 * @param i the index of the facing facet
 * @return a pointer to the facing facet
 */
SmartFacet* SmartPoint::getFaceingFacet(int i) {
    return facingFacets[i];
}

/**
 * the number of facing facets
 * @return the number of facets that face this point.
 */
int SmartPoint::numFacingFacets() const {
    return facingFacets.size();
}

/**
 * Removes all instances of a facet from the list of those that face this point.
 * @param facing the facet to be removed.
 */
void SmartPoint::removeFacingFacet(SmartFacet* facing) {
    facingFacets.erase
            (std::remove(facingFacets.begin(), facingFacets.end(), facing)
            , facingFacets.end());
}

/**
 * reserves space for facing facets
 * @param size the number of facing facets expected
 */
void SmartPoint::reserveFacingFacetSpace(int size) {
    facingFacets.reserve(size);
}
