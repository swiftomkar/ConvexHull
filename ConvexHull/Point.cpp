
/* 
 * File:   Point.cpp
 * Author: dov
 * 
 * Created on April 2, 2018, 1:02 PM
 */

#include "Point.h"


std::ostream& operator<<(std::ostream& s, const Point& p) {
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

