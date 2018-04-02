//
// Created by Omkar Desai on 4/2/2018.
//
#pragma once

#include "Point.h"
#include "Facet.h"
#include <iostream>
#include <vector>
class ConflictGraph:public Point, public Facet{
private:
    //for each point; make a list of facets that are in front of it
    std::vector<int> conflicts;

public:
    ConflictGraph(Point p){
        
    }
};