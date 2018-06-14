/**
 * @author Dov Neimand
 */
#include "Facet.h"
#include <assert.h>
#define self (*this)

/**
 * empty constructor.  The facet still needs to be loaded.
 */
Facet::Facet() {

}

/**
 * Constructor. Facet created with three points.  The points must be in counter
 *  clockwise order and coplanar.
 * @param a
 * @param b
 * @param c
 */
Facet::Facet(Point a, Point b, Point c) {
    reserve(3);
    push_back(a);
    push_back(b);
    push_back(c);
}

/**
 * A normal vector
 * @return a vector normal to this facet.  Note, it is not necessarily a unit 
 * vector.
 */
Point Facet::normal() const {
    return (self[2] - self[1]).cross(self[0] - self[1]);
}

/**
 * where the point is relative to the facet
 * @param p
 * @return positive if the facet faces the point, 0 if the point is on the facet
 * and negative otherwise.
 */
double Facet::relToFacet(const Point& p) const {
    return (p - self[0]).dot(normal());
}

/**
 * Does this facet face the provided point.
 * @param p the point
 * @return true if the facet faces the point, else false.
 */
bool Facet::faces(const Point& p) const {
    return relToFacet(p) > Point::EPSILON;
}

/**
 * Is the point on the plane.
 * @param p the point to be checked.
 * @return true if it's on the plane, false if not.
 */
bool Facet::onPlane(const Point& p) const {
    double op = relToFacet(p);
    return (op < 0 ? -op : op) < Point::EPSILON;
}

/**
 * Builds a side plane, a plane perpendicular to this one that runs along edge i
 * @param i the index of the edge of the new plane
 * @return a plane perpendicular to this one along edge i.
 */
Facet Facet::sidePlane(int i) const {
    Point top = self[i] + normal();
    Facet side;
    side.push_back(self[i]);
    assert(size() != 0);
    side.push_back(self[(i + 1) % size()]);
    side.push_back(top);
    return side;
}

/**
 * Does thge side plane on edge i face point p?
 * @param edge the edge the side plane is to be made on.
 * @param p the point it may or may not face.
 * @return true if the side plane faces the point, otherwise false
 */
bool Facet::edgeFaces(int edge, const Point& p) const {
    return sidePlane(edge % size()).faces(p);
}

/**
 * adds a point to the correct counter clockwise spot in the list of points.
 * @param p the point to be added.
 */
void Facet::addPoint(const Point& p) {

    for (int i = 0; i < size(); i++) {
        if (edgeFaces(i, p)) {
            while (edgeFaces(i + 1, p)) erase(begin() + (i + 1) % size());
            insert(begin() + (i + 1) % size(), p);
            break;
        }

    }
}

/**
 * outputs this facet on the given stream
 * @param os the stream to output the facet
 * @param f the facet to be output
 * @return the stream
 */
std::ostream& operator<<(std::ostream& os, std::vector<Point> f) {
    for (auto p : f) {
        os << "(" << p << "), ";
    }
    return os;
}

/**
 * inverts the order of the vertices.  The new normal will face the oppasite
 * direction of the old one.
 */
void Facet::flip() {
    int left = 0, right = size() - 1;
    while (left < right) {
        std::swap(self[left++], self[right--]);
    }
}

/**
 * outputs this facet to an stl
 * @param stlOut
 */
void Facet::stl(std::ofstream& stlOut) {
    for (int i = 0; i < size() - 2; i++) {
        stlOut << "facet normal " << normal().x << " " << normal().y << " " << normal().z << std::endl;
        stlOut << "\touter loop" << std::endl;
        
        self[0].stl(stlOut);
        self[i+1].stl(stlOut);
        self[i+2].stl(stlOut);
        
        stlOut << "\tendloop" << std::endl;
        stlOut << "endfacet" << std::endl;
    }
}
