//
//#include "DivAndConqMPI.h"
//#include "DivideAndConquerOpenMP.h"
//#include "GiftWrapping.h"
//
///**
// * The constructor
// * Assumes enough points to go around and that their are four machines.
// * Gives each machine an equal share of the work, then  gathers in the
// * Convex Hulls and gift wraps them using openMP.
// * @param points
// */
//
//DivAndConqMPI::DivAndConqMPI(vector<SmartPoint>& points) {
//    MPI_Init(NULL, NULL);
//    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);
//    MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);
//    MPI_Get_processor_name(processorName, &nameLength);
//
//
//    vector<double> outPoints;
//    outPoints.reserve(points.size()*3);
//
//    if (worldRank == 0) {
//        sort(points.begin(), points.end());
//        reserve(2 * points.size() * points.size());
//
//        int numSubHulls = maxNumSubHulls(points);
//        if (numSubHulls < worldSize) {
//            IncrementalMethod im(points);
//            importEntireHull(im);
//            return;
//        }
//        for (int i = 0; i < points.size(); i++) points[i].writeToVector(outPoints);
//    }
//
//    vector<double> inPoints = scatter(outPoints);
//
//    DivideAndConquerOpenMP localCH(inPoints);
//    inPoints.clear();
//    localCH.writeToVector(inPoints);
//
//    vector<double> returned = gather(inPoints);
//
//    if (worldRank == 0) {
//        int i = 0;
//        vector<ConvexHull> hulls;
//        for (int i = 0; i < worldSize; i++)
//            hulls.push_back(ConvexHull(returned, i));
//        
//        GiftWrapping gw1, gw2;
//#pragma omp parallel
//        {
//            if (omp_get_thread_num() == 0) gw1.build(hulls[0], hulls[1]);
//            if (omp_get_thread_num() == 1) gw2.build(hulls[2], hulls[3]);
//        }
//        GiftWrapping gw(gw1, gw2);
//        importEntireHull(gw);
//
//    }
//
//}
//
///**
// * runs MPI gather with a vector
// * @param v the vector to be gathered
// * @return the vector gathered
// */
//vector<double> DivAndConqMPI::gather(const vector<double>& v) {
//    const double* sendArray = &(v[0]);
//    int recieveCount = v.size();
//
//    vector<double> receive;
//    receive.resize(recieveCount * worldSize);
//    MPI_Gather(sendArray,
//            recieveCount,
//            MPI_DOUBLE,
//            &(receive[0]),
//            recieveCount,
//            MPI_DOUBLE,
//            ROOT,
//            MPI_COMM_WORLD);
//
//    return receive;
//}
///**
// * See MPI scatter.  Runs on a vector
// * @param v the vector to be scattered
// * @return the vector recieved
// */
//vector<double> DivAndConqMPI::scatter(const vector<double>& v) {
//    const double* sendArray = &(v[0]);
//    int recieveCount = 1 + (v.size() - 1) / worldSize;
//
//    vector<double> receive;
//    receive.resize(recieveCount);
//    MPI_Scatter(sendArray,
//            recieveCount,
//            MPI_DOUBLE,
//            &(receive[0]),
//            recieveCount,
//            MPI_DOUBLE,
//            ROOT,
//            MPI_COMM_WORLD);
//
//    return receive;
//}
//
//DivAndConqMPI::~DivAndConqMPI() {
//    MPI_Finalize();
//}
