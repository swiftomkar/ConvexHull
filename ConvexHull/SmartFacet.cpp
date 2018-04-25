/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SmartFacet.cpp
 * Author: dov
 * 
 * Created on April 25, 2018, 5:26 PM
 */

#include "SmartFacet.h"
#include "Facet.h"

void SmartFacet::bondPoint(const SmartPoint& p) {
    points.pop_back(p);
    p.facets.pop_back(*this);
}

void SmartFacet::initPoints(const std::vector<SmartPoint>& points) {
    for (int i = 0; i < points.size(); i++)
        if (faces(points[i])) bondPoint(points[i]);
}

void SmartFacet::decouple() {
    for (int i = 0; i < neigbors.size(); i++)
        for (int j = 0; j < neigbors[i].neigbors.size(); j++)
            if (neigbors[i].neigbors[j] == *this) {
                neigbors[i].neigbors.erase(neigbors[i].neigbors.begin() + j);
                break;
            }

    for (int i = 0; i < points.size(); i++)
        for (int j = 0; j < points[i].facets.size(); j++)
            if (points[i].facets[j] == *this) {
                points[i].facets.erase(points[i].facets.begin() + j);
                break;
            }

}

void SmartFacet::bondNieghborOneSide(int i, const SmartFacet& f) {
    while (i <= size()) neigbors.push_back(nullptr);
    neigbors[i] = f;
}




