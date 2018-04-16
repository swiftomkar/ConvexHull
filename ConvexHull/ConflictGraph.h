//
// Created by Omkar Desai on 4/2/2018.
//
#pragma once

#include "Facet.h"
#include <vector>

class ConflictGraph:public Point{
private:
    //for each point not in the polyhedron ; make a list of facets that are in front of it
    std::vector <Facet> ConGraph;
public:
    void UpdateGraph(Facet f);
    std::vector<Facet> GetConflictGraph();
    ~ConflictGraph();
};