
/* 
 * File:   Point.cpp
 * Author: dov
 * 
 * Created on April 2, 2018, 1:02 PM
 */

#include <cmath>

#include "Point.h"

SmartPoint::ostream& operator<<(SmartPoint::ostream& s, const Point& p) {
    return s << p.x << ", " << p.y << " , " << p.z;
}

double Point::operator[](int i) const {
    switch (i) {
        case 0: return x;
        case 1: return y;
        case 2: return z;
    }
}

double Point::dot(const Point& p) const {
    double dot = 0;
    for (int i = 0; i < 3; i++) dot += (*this)[i] * p[i];
    return dot;
}

Point Point::cross(const Point& p) const {
    return Point(y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x);
}

Point Point::operator*(double t) const {
    return Point(x*t, y*t, z * t);
}

Point Point::operator+(const Point& p) const {
    return Point(p.x + x, p.y + y, p.z + z);
}

Point Point::operator-(const Point& p) const {
    return (*this)+(p*-1);
}

double Point::distSquared(const Point& p) const {
    return ((*this) - p).magSquared();
}

bool Point::operator==(const Point& p) const{
    return distSquared(p) < Point::EPSILON;
}

Point Point::operator=(const Point& p) {
    x = p.x; y = p.y; z = p.z;
}


int colinear(const Point& a, const Point& b, const Point& c) {
    if (((b - a).cross(c - a)).magSquared() > Point::EPSILON) return -1;
    if (a.inBetween(b, c)) return 0;
    if (b.inBetween(a, c)) return 1;
    if (c.inBetween(a, b)) return 2;
}

double Point::magSquared() const {
    return x * x + y * y + z*z;
}

double Point::mag() const{
    return sqrtf(magSquared());
}

double Point::d(const Point& p) const{
    return ((*this) - p).mag();
}

bool Point::inBetween(const Point& a, const Point& b) const{
    double ab = a.distSquared(b);
    if (distSquared(a) < ab && distSquared(b) < ab) return true;
}

