

#include <cstdlib>

#include "ConvexHull.h"
#include "SmartPoint.h"

using namespace std;


int main(int argc, char** argv) {
    
    
    vector<SmartPoint> points;
    
    points.push_back(SmartPoint(0,0,1));
    points.push_back(SmartPoint(0,1,0));
    points.push_back(SmartPoint(1,0,0));
    points.push_back(SmartPoint(0,0,0));
    
    points.push_back(SmartPoint(-1,-1,-1));
    
        
    ConvexHull ch(points);
    
    

    return 0;
}

