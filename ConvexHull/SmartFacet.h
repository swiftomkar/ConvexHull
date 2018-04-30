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
    SmartPoint::vector<SmartFacet> neigbors;
    
    void bondPoint(SmartPoint& p);
    void initPoints(SmartPoint::vector<SmartPoint>& points);
    void decouple(SmartPoint& source);
    
    void bondNieghborOneSide(int i, SmartFacet& f);
    void disable();
    bool enabled();
    
private:
    friend SmartPoint;
    SmartPoint::vector<SmartPoint> points;    
    bool isEnabled = true;
    
};

#endif /* SMARTFACET_H */

