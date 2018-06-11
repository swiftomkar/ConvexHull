/**
 * @author Dov Neimand 
 */
#include "SmartPoint.h"
#include "SmartFacet.h"

/**
 * Is this point inside of all the facets it's keeping track of?
 * @return true if it is, false otherwise
 */
bool SmartPoint::inside() const{
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

