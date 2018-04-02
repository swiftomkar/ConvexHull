/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: dov
 *
 * Created on April 2, 2018, 1:02 PM
 */

#include <cstdlib>
#include "Point.h"
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
        
    Point x(1,0,0);
    Point y(0,1,0);
    cout<<x.cross(y);
    
    return 0;
}

