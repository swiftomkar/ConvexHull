//
// Created by Omkar Desai on 4/23/2018.
//

#pragma once

#include <iostream>
#include "Point.h"
#include <vector>
class GiftWrap:std::vector<Point> {
private:
    Point* pointList;
    int n;
    int checkPointOrientation(Point a,Point b,Point c){}


public:
    GiftWrap(Point* point,int n):pointList(new Point[n]),n(n){
        for(int i=0;i<n;i++){
            pointList[i]=point[i];
        }
        Wrap();
    }

    void Wrap() {
        // There must be at least 3 points
        if (n < 3)
            return;
        //find the leftmost point
        int l = 0;
        for (int i = 1; i < n; i++) {
            if (pointList[i].x < pointList[l].x) {
                l = i;
            }
        }

       //keep checking for the next counterclockwise point ;starts from the leftmost point in that section
        int p = l, q;
        do {
            // Add first point to result
            push_back(pointList[p]);

            //search point which is most counterclkwise
            q = (p + 1)%n;
            for (int i = 0; i < n; i++) {
                // If i is counterclockwise than current q, then
                // update q
                if (checkPointOrientation()=counterclockwise {
                    q = i;
                }
            }
            p = q;
        }while (p != l);
    }
};
