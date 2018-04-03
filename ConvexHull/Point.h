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
private:
    double x,y,z;
public:
    Point(double x=0, double y=0, double z=0);
    Point(const Point& orig);
    Point cross(Point p);
    ~Point();
    friend std::ostream& operator<<(std::ostream& s, const Point& p);
    double operator[](int i);
    double operator *(Point p);
};

#endif /* POINT_H */

