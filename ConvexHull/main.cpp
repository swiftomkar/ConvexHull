
/* 
 * File:   main.cpp
 * Author: dov
 *
 * Created on March 28, 2018, 5:03 PM
 */

#include <cstdlib>
#include <cmath>
#include <iostream>
#include "Facet.h"

using namespace std;

void testNorm() {
    Facet f;

    f.push_back(Point(0, 0, 0));
    f.push_back(Point(0, 1, 0));
    f.push_back(Point(1, 0, 0));

    cout << f.faces(Point(1, 0, 1));
}


int main(int argc, char** argv) {

    Point a(0, 0, 0);
    Point b(0, 0, 1);
    Point c(1, 0, 0);
    
    Facet f;
    f.push_back(a);f.push_back(b); f.push_back(c);
    
    Point d(0,1,0);
    
    f.addPoint(d);
    
    return 0;
}

