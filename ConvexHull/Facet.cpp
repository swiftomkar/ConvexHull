
/* 
 * File:   Facet.cpp
 * Author: dov
 * 
 * Created on April 4, 2018, 5:31 PM
 */

#include "Facet.h"


Point Facet::normal() const{
    return ((*this)[0]-(*this)[1]).cross((*this)[2] - (*this)[1]);
}


bool Facet::faces(const Point& p) const{
    return (p - (*this)[0]).dot(normal()) > 0;
}
