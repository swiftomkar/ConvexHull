//
//#pragma once
//
//#include "ConvexHull.h"
//#include <mpi.h>
//
///**
// * @author: Dov Neimand
// * This class uses mpi together with Chan's algorithm to create a comvex hull. 
// * It's written for four computers.
// */
//class DivAndConqMPI : ConvexHull {
//public:
//    DivAndConqMPI(vector<SmartPoint>& points);
//
//    ~DivAndConqMPI();
//
//
//private:
//    int worldSize;
//    int worldRank;
//    char processorName[MPI_MAX_PROCESSOR_NAME];
//    int nameLength;
//    const int ROOT = 0;
//
//    int worldSize;
//
//    vector<double> scatter(const vector<double>& v);
//    vector<double> gather(const vector<double>& v);
//
//};
//
