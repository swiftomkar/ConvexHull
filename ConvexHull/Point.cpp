/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Point.cpp
 * Author: dov
 * 
 * Created on April 2, 2018, 1:02 PM
 */

#include "Point.h"


Point::Point(double x, double y, double z):x(x),y(y),z(z){}

Point::Point(const Point& orig):x(orig.x),y(orig.y),z(orig.z) {
}

Point::~Point() {}

std::ostream& operator<<(std::ostream& s, const Point& p) {
    s << p.x << ", " << p.y << " , " << p.z;
    return s;
}

double Point::operator[](int i) {
    switch(i){
        case 0: return x;
        case 1: return y;
        case 2: return z;
    }
}

double Point::operator * (Point p) {
    double dot = 0;
    for(int i = 0; i < 3; i++)
        dot += (*this)[i]*p[i];
    return dot;
}

Point Point::cross(Point p) {
    return Point(y*p.z - z*p.y, z*p.x - x*p.z, x*p.y-y*p.x);
}
