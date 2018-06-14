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
#include <omp.h>
#define self (*this) 
using namespace std;

/**
 * The constructor
 * @param ch
 */
HullProj2d::HullProj2d(ConvexHull& ch) {
    build(ch);
}


/**
 * builds the hull
 * @param ch
 */
void HullProj2d::build(ConvexHull& ch) {
    int k = 0;

    reserve(2 * ch.numHullPoints());

    // Sort points lexicographically
    ch.sortHullPoints();

    // Build lower hull
    for (int i = 0; i < ch.numHullPoints(); ++i) {
        while (k >= 2 && cross2d(self[k - 2], self[k - 1], ch.getHullPoint(i)) <= 0) k--;
        while (k >= size()) push_back(Point(0, 0, 0));
        self[k++] = ch.getHullPoint(i);
    }

    // Build upper hull
    for (int i = ch.numHullPoints() - 1, t = k + 1; i > 0; --i) {
        while (k >= t && cross2d(self[k - 2], self[k - 1], ch.getHullPoint(i - 1)) <= 0) k--;
        while (k >= size()) push_back(Point(0, 0, 0));
        self[k++] = ch.getHullPoint(i - 1);
    }

    resize(k - 1);
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
int HullProj2d::minX() const {
    int min = 0;

#pragma omp parallel
    {
        int id = omp_get_thread_num();
        int numTh = omp_get_num_threads();
    for (int i = id; i < size(); i+=numTh) {
            if (self[i].x < self[min].x) {
#pragma omp critical
                min = i;
            }
        }
    }
    
    return min;
}

/**
 * the index of the right most point on the hull.
 * @return 
 */
int HullProj2d::maxX() const{
    int max = 0;

#pragma omp parallel
    {
        int id = omp_get_thread_num();
        int numTh = omp_get_num_threads();
    for (int i = id; i < size(); i+=numTh) {
            if (self[i].x > self[max].x) {
#pragma omp critical
                max = i;
            }
        }
    }
    
    return max;
}

/**
 * Does the selected edge face the point p?
 * @param edge the edge to check against.
 * @param p the point that might be faced.
 * @return true if the edge faces the point, false otherwise.
 */
bool HullProj2d::edgeFaces(int edge, const Point& p) const {

    if (edge < 0) edge = size() + edge;
    Facet side;
    side.push_back(self[edge].xyProj());
    side.push_back(self[(edge + 1) % size()].xyProj());
    side.push_back(Point(self[edge].x, self[edge].y, 1));

    return side.faces(p);
}
