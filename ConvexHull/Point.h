/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Point.h
 * Author: dov
 *
 * Created on April 2, 2018, 1:02 PM
 */



#ifndef POINT_H
#define POINT_H

#include <iostream>

class Point {
public:
    //Point();
    

    const double x, y, z;
    static constexpr double EPSILON = 1e-7;
    
    Point(const Point& orig): x(orig.x), y(orig.y), z(orig.z){}    
    Point(double x, double y, double z) : x(x), y(y), z(z){}

    friend std::ostream& operator<<(std::ostream& s, const Point& p);
    double operator[](int i) const;
    double dot(const Point& p) const;
    Point cross(const Point& p) const;
    
    Point operator +(const Point& p) const;
    Point operator -(const Point& p) const;
    Point operator *(double t) const;
//    bool operator ==(Point p);
    
    friend int colinear(Point a, Point b, Point c);
    double mag();
    double magSquared();
    double d(Point p);
    double distSquared(Point p);
   
private:
    bool inBetween(Point a, Point b);

};

#endif /* POINT_H */
