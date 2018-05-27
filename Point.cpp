
/**
 * @author Dov Neimand
 */

#include <cmath>
#include "Point.h"

/**
 * Outputs the x, y, z values of a point
 * @param s the stream to output the point.
 * @param p the point to be output
 * @return the stream
 */
std::ostream& operator<<(std::ostream& s, const Point& p) {
    return s << p.x << ", " << p.y << " , " << p.z;
}

/**
 * The i'th value of the point 0 -> x, 1 -> y, 2 -> z
 * @param i the index of the value of the point
 * @return the value at index i
 */
double Point::operator[](int i) const {
    switch (i) {
        case 0: return x;
        case 1: return y;
        case 2: return z;
    }
}

/**
 * dot product
 * @param p 
 * @return the dot product
 */
double Point::dot(const Point& p) const {
    double dot = 0;
    for (int i = 0; i < 3; i++) dot += (*this)[i] * p[i];
    return dot;
}

/**
 * cross product
 * @param p
 * @return cross product
 */
Point Point::cross(const Point& p) const {
    return Point(y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x);
}

/**
 * scalar multiplication
 * @param t the scalar
 * @return  the multiplication
 */
Point Point::operator*(const double t) const {
    return Point(x*t, y*t, z * t);
}

/**
 * vector addition
 * @param p the vector
 * @return the additon
 */
Point Point::operator+(const Point& p) const {
    return Point(p.x + x, p.y + y, p.z + z);
}

/**
 * vector subtraction
 * @param p
 * @return 
 */
Point Point::operator-(const Point& p) const {
    return (*this)+(p*-1);
}

/**
 * the distance between two points squared.
 * @param p the other point
 * @return the distance squared
 */
double Point::distSquared(const Point& p) const {
    return ((*this) - p).magSquared();
}

/**
 * are these two points the same (or nearly the same depending on epsilon)
 * @param p
 * @return true of they're the same.  Otherwise, false.
 */
bool Point::operator==(const Point& p) const{
    return distSquared(p) < Point::EPSILON;
}

/**
 * assign value
 * @param p assigns to this vector the values in p.
 * @return *this
 */
Point Point::operator=(const Point& p) {
    x = p.x; y = p.y; z = p.z;
    return *this;
}


/**
 * Are these three points on the same line.
 * @param a
 * @param b
 * @param c
 * @return the index of the middle point, where 0 -> a and 1 -> b, and 2 -> c.
 * Returns -1 if they are not colinear.
 */
int colinear(const Point& a, const Point& b, const Point& c) {
    if (((b - a).cross(c - a)).magSquared() > Point::EPSILON) return -1;
    if (a.inBetween(b, c)) return 0;
    if (b.inBetween(a, c)) return 1;
    if (c.inBetween(a, b)) return 2;
}

/**
 * the distance from this point to the origin sqwuared.
 * @return the distance from this point to the origin sqwuared. 
 */
double Point::magSquared() const {
    return x * x + y * y + z*z;
}

/**
 * the magnitude
 * @return the magnitude
 */
double Point::mag() const{
    return sqrtf(magSquared());
}

/**
 * the distance between two points.
 * @param p the second point
 * @return the distance from this point to p
 */
double Point::d(const Point& p) const{
    return ((*this) - p).mag();
}

/**
 * is this point in between a and b, two other points.  This really only makes
 * sense if all three points are colinear.
 * @param a 
 * @param b
 * @return true if this point is in between a and b, otherwise false.
 */
bool Point::inBetween(const Point& a, const Point& b) const{
    double ab = a.distSquared(b);
    if (distSquared(a) < ab && distSquared(b) < ab) return true;
}

