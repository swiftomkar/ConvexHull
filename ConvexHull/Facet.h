/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Facet.h
 * Author: dov
 *
 * Created on April 2, 2018, 1:04 PM
 */



#ifndef FACET_H
#define FACET_H

#include "Point.h"

class Facet {
public:
    Facet();
    Facet(const Facet& orig);
    virtual ~Facet();
        
    bool faces(Point p);
    Point normal();
private:

};

#endif /* FACET_H */

