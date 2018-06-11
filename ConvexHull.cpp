
#include "ConvexHull.h"
#include <iostream>
#include <unordered_set>
#include <assert.h>
#define self (*this)

/**
 * A printable list of the facets in ch
 * @param os an ostream to output the convex hull desacription
 * @param ch the convex hull to be printed.
 * @return the ostream.
 */
ostream& operator<<(ostream& os, ConvexHull& ch) {
    os << "Facets:" << endl;
    for (int i = 0; i < ch.size(); i++) {
        os << ch[i] << endl;
    }
    os << "Points:" << endl;
    for (int i = 0; i < ch.hullPoints.size(); i++)
        os << "(" << ch.hullPoints[i] << "), ";

    os << endl << "number of facets = " << ch.size() << endl;
    os << "number of points = " << ch.hullPoints.size() << endl;
    os << "healthy neighbors check: " << (ch.testNeighbors() ? "success" : "fail") << endl;
    return os;
}

/**
 * Creates a list of all the points in the hull.
 */
void ConvexHull::setHullPoints() {

    struct hash {

        double operator()(const Point &p) const {
            size_t h1 = std::hash<double>()(p.x);
            size_t h2 = std::hash<double>()(p.y);
            size_t h3 = std::hash<double>()(p.z);
            return (h1 ^ (h2 << 1)) ^ h3;
        }
    };

    
    unordered_set<Point, hash> potPoints;
    for (int i = 0; i < size(); i++)
        for (int j = 0; j < self[i].size(); j++) 
                potPoints.insert(self[i][j]);
            

    hullPoints.reserve(3 * size());
    unordered_set<Point, hash> ::iterator itr;
    for (itr = potPoints.begin(); itr != potPoints.end(); itr++)
        hullPoints.push_back(*itr);
}

/**
 * find a facet and index for this point in the hull
 * @param search the hull to be searched
 * @param facetInd this reference will be assigned the index of the facet the point 
 * is in.
 * @param pointInd this reference will be assigned the index of the point in
 * the facet it's found in.
 */
void ConvexHull::locPoint(const Point& search, int& facetInd, int& pointInd) const {

    for (int i = 0; i < self.size(); i++)
        for (int j = 0; j < self[i].size(); j++)
            if (self[i][j] == search) {
                facetInd = i;
                pointInd = j;
                return;
            }
}

/**
 * removes from this all facets marked as garbage.
 */
void ConvexHull::removeDisabledFacets() {
    int shift = 0;
    int i = 0;
    while (i + shift < size()) {
        if (self[i + shift].enabled()) {
            self[i + shift].changeOfAddress(shift);
            self[i] = self[i + shift];
            i++;
        } else shift++;
    }
    for (int i = 0; i < shift; i++) pop_back();
}

/**
 * checks to see if the neighbors are properly aligned
 * @return 
 */
bool ConvexHull::testNeighbors() {

    for (int i = 0; i < size(); i++) {
        if ((*this)[i].enabled() && !(*this)[i].testNeighbors()) {
            cout << (*this)[i] << " failed neighbor test";
            return false;
        }
    }
    return true;
}

/**
 * TO confirm that hull is convex.  Should only be used to double check results,
 * not compute them.
 * @return 
 */
bool ConvexHull::isConvex(bool mute) {
    setHullPoints();

    if (size() == 0) {
        if (!mute) cerr << "hull is empty" << endl;
        return false;
    }
    for (int i = 0; i < hullPoints.size(); i++)
        for (int j = 0; j < size(); j++)
            if ((*this)[j].faces(hullPoints[i])) {
                if (!mute) {
                    cerr << (*this)[j] << " faces " << hullPoints[i] << endl;
                    cerr << "rel to facet = " << (*this)[j].relToFacet(hullPoints[i]) << endl;
                    cerr << "generating stl" << endl;
                    stl();
                }
                return false;
            }
    return true;
}

/**
 * Sets up the first facet and adds it to this. This facet is selected from
 * the first points.
 * @param points The points the convex hull is meant to encompass and the first
 * facet taken from.
 * @param start the location in the vector the facet should start being taken 
 * from
 * @param add should the faced be added to this convex hull?
 * @return the index of the first point not in the first facet.
 */
int ConvexHull::findFirstFacet(const vector<SmartPoint>& points, int start, bool add) {

    SmartFacet facet1(10);
    int pointsIndex = start;
    facet1.push_back(points[pointsIndex++]);

    //make a line    
    for (; pointsIndex < points.size() && (points[pointsIndex] == points[start])
            ; pointsIndex++);
    facet1.push_back(points[pointsIndex++]);

    //make a triangle
    for (; pointsIndex < points.size(); pointsIndex++) {
        int j = colinear(facet1[0], facet1[1], points[pointsIndex]);
        if (j == -1) break;
        facet1.erase(facet1.begin() + j);
    }

    facet1.push_back(points[pointsIndex++]);

    //makes a polygon if necessary
    for (; pointsIndex < points.size() && facet1.onPlane(points[pointsIndex]); pointsIndex++) {
        facet1.addPoint(points[pointsIndex]);
        assert(pointsIndex < start + 50);
    }

    if (add) push_back(facet1);

    if (pointsIndex >= points.size()) {
        if (start == 0)
            cerr << "IncrementalMethod::firstFacet " <<
                "- no polyhedron can be made from these points." << endl;
    }
    return pointsIndex;
}

/**
 * Brings in the facets from the given convex hull to this one.
 * Will not import facets marked as disabled.  Updates addresses of neighbors
 * as it goes.
 * @param source the facets to bring over.
 */
void ConvexHull::importFacets(ConvexHull& source) {
    for (int i = 0; i < source.size(); i++)
        if (source[i].enabled()) {
            source[i].changeOfAddress(&self[size()]);
            push_back(source[i]);
        }
}

/**
 * adds to this an entire hull and its points.
 * @param source
 */
void ConvexHull::importEntireHull(ConvexHull& source) {
    importFacets(source);
    hullPoints.reserve(source.hullPoints.size());
    for (int i = 0; i < source.hullPoints.size(); i++)
        hullPoints.push_back(source.hullPoints[i]);
}

/**
 * Writes all the facets of this convex hull to a vector for transport.  For
 * details, see smart facet documentation.
 * @param vec the vector to be written to.
 */
void ConvexHull::writeToVector(vector<double>& vec) {
    for (int i = 0; i < size(); i++) (*this)[i].appendToVector(vec);
    vec.push_back(NAN);
    for (int i = 0; i < hullPoints.size(); i++)
        hullPoints[i].writeToVector(vec);
}

/**
 * Constructor
 * builds a convex hull from a vector of doubles.  See documentation for
 * SmartFacet::appendToVector.
 * @param vec the vector to build the hull from.
 */
ConvexHull::ConvexHull(const vector<double>& vec) {
    reserve(vec.size() / 3 + 1);
    int i = 0;
    for (; vec[i] == vec[i] || vec[i - 1] == vec[i - 1]; i++)
        push_back(SmartFacet(vec, i, &(*this)[size()]));

    for (i++; i < vec.size(); i += 3)
        hullPoints.push_back(SmartPoint(vec, i));

}

/**
 * writes this convex hull to an stl
 */
void ConvexHull::stl() {
    if (stlPrint < 20) {
        ofstream stlOut;
        cout << "generating " << "ConvexHull" + to_string(stlPrint) + ".stl"<<endl;
        stlOut.open("ConvexHull" + to_string(stlPrint++) + ".stl");
        stlOut << "solid ConvexHull" << endl;
        for (int i = 0; i < size(); i++) (*this)[i].stl(stlOut);
        stlOut << "endsolid ConvexHull" << endl;
        stlOut.close();
    }
}
