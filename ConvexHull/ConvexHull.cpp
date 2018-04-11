/* 
 * File:   ConvexHull.cpp
 * Author: eneimand
 * 
 * Created on April 9, 2018, 2:07 PM
 */

#include "ConvexHull.h"

ConvexHull::ConvexHull(std::vector<Point> points) {
    Facet triangle;
    int i = 0;
    triangle.push_back(points[i++]);
        
    for(; i < points.size() && points[i] == points[0]; i++);
    
    triangle.push_back(points[i++]);
    
//    for(int j = colinear(triangle[0], triangle[1], points[i]); i < points.size() && j !=-1; i++, );
    
        
}



//
//ConvexHull::ConvexHull(const ConvexHull& orig) {
//}
//
//ConvexHull::~ConvexHull() {
//}

