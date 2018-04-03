//
// Created by Omkar Desai on 4/2/2018.
//
#pragma once

#include "Point.h"
#include "Facet.h"
#include <iostream>
#include <vector>
class ConflictGraph:public Facet{
private:
    //for each point; make a list of facets that are in front of it
    std::vector <Facet> ConGraph;
    bool isEmpty;
public:
    ConflictGraph(Point p) {
        isEmpty = true;
        UpdateGraph(p);
    }
    void UpdateGraph(Point p){
        for(int i=0;i<Facet::numFacets();i++) {
            if (Facet::Facetlist[i].Faces(*this))//assuming we have a list of all facets
                ConGraph.push_back(Facet::Facetlist[i]);
        }
    }
    std::vector<Facet> GetConflictGraph(){
        return this->ConGraph;
    }
};
