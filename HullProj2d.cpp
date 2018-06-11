/**
 * Much of the code in this file was adapted from Wikipedia.
 * https://en.wikibooks.org/wiki/Algorithm_Implementation/Geometry/Convex_hull/Monotone_chain
 * 
 */
#include "HullProj2d.h"
#include <unordered_set>
#include <algorithm>
#include <vector>
#include <assert.h>
#define self (*this) 
using namespace std;

/**
 * The constructor
 * @param ch
 */
HullProj2d::HullProj2d(ConvexHull& ch) {
    int k = 0;

    reserve(2 * ch.hullPoints.size());
    
//    cout << "HullProj2d\nbefore projecting" << endl;
//    for(int i = 0; i < ch.hullPoints.size(); i++)
//        cout << ch.hullPoints[i] <<"\t";
//    cout << endl;
    

    // Sort points lexicographically
    sort(ch.hullPoints.begin(), ch.hullPoints.end());

    // Build lower hull
    for (int i = 0; i < ch.hullPoints.size(); ++i) {
        while (k >= 2 && cross2d(self[k - 2], self[k - 1], ch.hullPoints[i]) <= 0) k--;
        while (k >= size()) push_back(Point(0, 0, 0));
        self[k++] = ch.hullPoints[i];
    }

    // Build upper hull
    for (int i = ch.hullPoints.size() - 1, t = k + 1; i > 0; --i) {
        while (k >= t && cross2d(self[k - 2], self[k - 1], ch.hullPoints[i - 1]) <= 0) k--;
        while (k >= size()) push_back(Point(0, 0, 0));
        self[k++] = ch.hullPoints[i - 1];
    }

    resize(k - 1);
    
    
//    cout << "after projecting" << endl;
//    for(int i = 0; i < ch.hullPoints.size(); i++)
//        cout << self[i] <<"\t";
//    cout << endl;
}

/**
 * Cross product that only looks as the x and y coordinates.  That is, the
 * cross product of the projection onto the xy plane of A-O and B-O.
 * @param O
 * @param A
 * @param B
 * @return (A-O)x(B-O)
 */
double HullProj2d::cross2d(const Point &O, const Point &A, const Point &B) {
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

/**
 * the index of the left most point on the hull.
 * @return 
 */
int HullProj2d::minX() {
    int min = 0;
    for(int i = 1; i < size(); i++)
        if(self[min].x > self[i].x) min = i;
    return min;
}
/**
 * the index of the right most point on the hull.
 * @return 
 */
int HullProj2d::maxX() {
    int min = 0;
    for(int i = 1; i < size(); i++)
        if(self[min].x <= self[i].x) min = i;
    return min;
}

/**
 * Does the selected edge face the point p?
 * @param edge the edge to check against.
 * @param p the point that might be faced.
 * @return true if the edge faces the point, false otherwise.
 */
bool HullProj2d::edgeFaces(int edge, const Point& p) const {
    
    if(edge < 0) edge = size() + edge;
    Facet side;
    side.push_back(self[edge].xyProj());
    side.push_back(self[(edge + 1) % size()].xyProj());
    side.push_back(Point(self[edge].x, self[edge].y, 1));
    
    return side.faces(p);
}
