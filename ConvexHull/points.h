//
// Created by Omkar Desai on 2/26/2018.
//
#pragma once

#include <iostream>
using namespace std;
class Point{
public:
    double x, y, z;
    point(double x=0, double y=0, double z=0):x(x),y(y),z(z){}
    friend ostream& operator << (ostream& s,const point& p){
        s<<p.x<<", "<<p.y<<" , "<<p.z;
        return s;
    }
};
