//
// Created by Omkar Desai on 4/16/2018.
//
#include "ConflictGraph.h"

void ConflictGraph::UpdateGraph(Facet f){
    if (f.faces(*this))//assuming we have a list of all facets
        ConGraph.push_back(f);
}
std::vector<Facet> ConflictGraph::GetConflictGraph(){
    return this->ConGraph;
}
ConflictGraph::~ConflictGraph(){
    delete ConGraph;
}