/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SmartFacet.h
 * Author: dov
 *
 * Created on April 25, 2018, 5:26 PM
 */

#ifndef SMARTFACET_H
#define SMARTFACET_H

#include "smartPoint.h"


class SmartFacet: public Facet {
public:
    void bondPoint(const SmartPoint& p);
    void initPoints(const std::vector<SmartPoint>& points);
    void decouple();
    void bondNeighbor(const SmartFacet& f);
    void bondNieghborOneSide(int i, const SmartFacet& f);
private:
    friend SmartPoint;
    std::vector<SmartPoint> points;
    std::vector<SmartFacet> neigbors;
    
};

#endif /* SMARTFACET_H */

