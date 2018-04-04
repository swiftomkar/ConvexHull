/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Facet.h
 * Author: dov
 *
 * Created on April 4, 2018, 5:31 PM
 */

#ifndef FACET_H
#define FACET_H


#include <vector>
#include "Point.h"

class Facet: public std::vector<Point> {
public: 

    bool faces(const Point& p) const;
    Point normal() const;

};

#endif /* FACET_H */
