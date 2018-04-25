
/* 
 * File:   Facet.cpp
 * Author: dov
 * 
 * Created on April 4, 2018, 5:31 PM
 */

#include "Facet.h"

Point Facet::normal() const {
    return ((*this)[0]-(*this)[1]).cross((*this)[2] - (*this)[1]);
}

bool Facet::faces(const Point& p) const {
    return (p - (*this)[0]).dot(normal()) > 0;
}

bool Facet::onPlane(const Point& p) const {
    return abs((p - (*this)[0]).dot(normal())) < Point::EPSILON;
}

Facet Facet::sidePlane(int i) const {
    Point top = ((*this)[(i + 2) % size()]-(*this)[(i + 1) % size()]).cross((*this)[i]-(*this)[(i + 1) % size()]);
    Facet side;
    side.push_back((*this)[i]);
    side.push_back(top);
    side.push_back((*this)[(i + 1) % size()]);

    return side;
}

bool Facet::edgeFaces(int edge, const Point& p) const {
    return sidePlane(edge).faces(p);
}

void Facet::addPoint(const Point& p) {
    for (int i = 0; i < size(); i++) {
        if (edgeFaces(i, p)) {
            while (edgeFaces(i + 1, p)) erase(begin() + i + 1);
            insert(begin() + i + 1, p);
        }
        
    }
}

std::ostream& operator<<(std::ostream& os, Facet f) {
    for (auto p : f) {
        os << "(" << p << "), ";
    }
    return os;
}

//void Facet::flip() {
//    vector<Point> temp(*this);
//    this->clear();
//    for(int i = temp.size() - 1; i >= 0; i--){        
//        push_back(temp[i]);
//    }
//}
void Facet::flip() {
    int left = 0, right = size() - 1;
    while(left < right) {
        std::swap((*this)[left++], (*this)[right--]);
    }
}

bool Facet::operator==(const Facet& f) const {
    if(size() != f.size()) return false;
    for(int i = 0; i < size(); i++)
        if(!((*this)[i] ==f[i])) return false;
    return true;
}
