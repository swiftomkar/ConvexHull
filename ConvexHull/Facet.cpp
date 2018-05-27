/**
 * @author Dov Neimand
 */
#include "Facet.h"

/**
 * A normal vector
 * @return a vector normal to this facet.  Note, it is not necessarily a unit 
 * vector.
 */
Point Facet::normal() const {
    return ((*this)[0]-(*this)[1]).cross((*this)[2] - (*this)[1]);
}

/**
 * Does this facet face the provided point.
 * @param p the point
 * @return true if the facet faces the point, else false.
 */
bool Facet::faces(const Point& p) const {
    return (p - (*this)[0]).dot(normal()) > Point::EPSILON;
}

/**
 * Is the point on the plane.
 * @param p the point to be checked.
 * @return true if it's on the plane, false if not.
 */
bool Facet::onPlane(const Point& p) const {
    return abs((p - (*this)[0]).dot(normal())) < Point::EPSILON;
}

/**
 * Builds a side plane, a plane perpendicular to this one that runs along edge i
 * @param i the index of the edge of the new plane
 * @return a plane perpendicular to this one along edge i.
 */
Facet Facet::sidePlane(int i) const {
    Point top = ((*this)[(i + 2) % size()]-(*this)[(i + 1) % size()]).cross((*this)[i]-(*this)[(i + 1) % size()]);
    Facet side;
    side.push_back((*this)[i]);
    side.push_back(top);
    side.push_back((*this)[(i + 1) % size()]);

    return side;
}

/**
 * Does thge side plane on edge i face point p?
 * @param edge the edge the side plane is to be made on.
 * @param p the point it may or may not face.
 * @return true if the side plane faces the point, otherwise false
 */
bool Facet::edgeFaces(int edge, const Point& p) const {
    return sidePlane(edge).faces(p);
}

/**
 * adds a point to the correct counter clockwise spot in the list of points.
 * @param p the point to be added.
 */
void Facet::addPoint(const Point& p) {
    for (int i = 0; i < size(); i++) {
        if (edgeFaces(i, p)) {
            while (edgeFaces(i + 1, p)) erase(begin() + i + 1);
            insert(begin() + i + 1, p);
        }
        
    }
}

/**
 * outputs this facet on the given stream
 * @param os the stream to output the facet
 * @param f the facet to be outputed
 * @return the stream
 */
std::ostream& operator<<(std::ostream& os, Facet f) {
    for (auto p : f) {
        os << "(" << p << "), ";
    }
    return os;
}

/**
 * inverts the order of the vertices
 */
void Facet::flip() {
    int left = 0, right = size() - 1;
    while(left < right) {
        std::swap((*this)[left++], (*this)[right--]);
    }
}

