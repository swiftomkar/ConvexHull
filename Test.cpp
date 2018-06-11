
#include "Test.h"
#include "DivideAndConquerOpenMP.h"
#include "IncrementalMethod.h"
#include "GiftWrapping.h"

void Test::incrementalMethod1() {
    vector<SmartPoint> points;

    points.push_back(SmartPoint(0, 0, 1));
    points.push_back(SmartPoint(0, 1, 0));
    points.push_back(SmartPoint(1, 0, 0));
    points.push_back(SmartPoint(0, 0, 0));

    points.push_back(SmartPoint(1, 1, 1));
    points.push_back(SmartPoint(-5, -5, -5));
    points.push_back(SmartPoint(-8, -8, -8));
    points.push_back(SmartPoint(-3, -3, -3));
    //    
    points.push_back(SmartPoint(10, 0, 0));

    std::random_shuffle(points.begin(), points.end());

    IncrementalMethod ch(points);

    cout << ch;

}

void Test::giftWrapping1() {
    vector<SmartPoint> points;
    points.push_back(SmartPoint(-1, 1, 0));
    points.push_back(SmartPoint(-1, -1, 0));
    points.push_back(SmartPoint(-1, .5, 1));
    points.push_back(SmartPoint(-1, -.5, 1));
    points.push_back(SmartPoint(-1, -.5, -1));
    points.push_back(SmartPoint(-1, .5, -1));
    points.push_back(SmartPoint(-10, 0, 0));

    IncrementalMethod hexToPoint(points);

    points.clear();

    points.push_back(SmartPoint(1, 0, 1));
    points.push_back(SmartPoint(1, 0, -1));
    points.push_back(SmartPoint(1, 1, 0));
    points.push_back(SmartPoint(1, -1, 0));
    points.push_back(SmartPoint(1.1, 0, 0));
    points.push_back(SmartPoint(.9, 0, .1));
    points.push_back(SmartPoint(.9, 0, -.1));

    IncrementalMethod quadToPoint(points);

    GiftWrapping gw(hexToPoint, quadToPoint);

    cout << gw << endl;
    gw.stl();
}

void Test::giftWrapping2() {
    vector<SmartPoint> points;
    points.push_back(SmartPoint(-1, 10, 0));
    points.push_back(SmartPoint(-1, -10, 0));
    points.push_back(SmartPoint(-1, 0, 10));
    points.push_back(SmartPoint(-1, 0, -10));
    points.push_back(SmartPoint(-5, 0, 0));

    IncrementalMethod bigQuadToPoint(points);

    points.clear();

    points.push_back(SmartPoint(1, 1, 0));
    points.push_back(SmartPoint(1, -1, 0));
    points.push_back(SmartPoint(1, 0, 1));
    points.push_back(SmartPoint(1, 0, -1));
    points.push_back(SmartPoint(5, 0, 0));


    IncrementalMethod smallQuadToPoint(points);

    GiftWrapping gw(bigQuadToPoint, smallQuadToPoint);

    cout << gw << endl;
    
    gw.stl();
}

void Test::giftWrapping3() {
    vector<SmartPoint> points;
    points.push_back(SmartPoint(-1, 1, 0));
    points.push_back(SmartPoint(-1, -1, 0));
    points.push_back(SmartPoint(-1, 0, 1));
    points.push_back(SmartPoint(-1, 0, -1));
    points.push_back(SmartPoint(-10, 0, 0));

    IncrementalMethod bigQuadToPoint(points);

    points.clear();

    points.push_back(SmartPoint(1, 10, 0));
    points.push_back(SmartPoint(1, -10, 0));
    points.push_back(SmartPoint(1, 0, 10));
    points.push_back(SmartPoint(1, 0, -10));
    points.push_back(SmartPoint(2, 0, 0));


    IncrementalMethod smallQuadToPoint(points);

    GiftWrapping gw(bigQuadToPoint, smallQuadToPoint);

    cout << gw << endl;
    gw.stl();
}

void Test::giftWrapping4() {
    vector<SmartPoint> points;
    points.push_back(SmartPoint(1, 10, 0));
    points.push_back(SmartPoint(1, -10, 0));
    points.push_back(SmartPoint(1, 0, 10));
    points.push_back(SmartPoint(1, 0, -10));
    points.push_back(SmartPoint(5, 0, 0));

    IncrementalMethod bigQuadToPoint(points);

    points.clear();

    points.push_back(SmartPoint(-1, 1, 0));
    points.push_back(SmartPoint(-1, -1, 0));
    points.push_back(SmartPoint(-1, 0, 1));
    points.push_back(SmartPoint(-1, 0, -1));
    points.push_back(SmartPoint(-5, 0, 1));
    points.push_back(SmartPoint(-5, 0, -1));

    IncrementalMethod smallQuadToPoint(points);

    GiftWrapping gw(smallQuadToPoint, bigQuadToPoint);

    cout << gw << endl;
    gw.stl();
}


void Test::giftWrapping5() {
    vector<SmartPoint> points;
    points.push_back(SmartPoint(-1, -1, 10));
    points.push_back(SmartPoint(-1, 1, 10));
    points.push_back(SmartPoint(-1, -1, -10));
    points.push_back(SmartPoint(-1, 1, -10));
    points.push_back(SmartPoint(-1.1, 0, 0));

    IncrementalMethod bigQuadToPoint(points);

    points.clear();

    points.push_back(SmartPoint(1, -10, 1));
    points.push_back(SmartPoint(1, 10, 1));
    points.push_back(SmartPoint(1, -10, -1));
    points.push_back(SmartPoint(1, 10, -1));
    points.push_back(SmartPoint(200, 0, 0));


    IncrementalMethod smallQuadToPoint(points);

    GiftWrapping gw(bigQuadToPoint, smallQuadToPoint);

    cout << gw << endl;
    gw.stl();
}


void Test::divideAndConquerOpenMP1() {
    vector<SmartPoint> points;
    points.push_back(SmartPoint(-1, 1, 0));
    points.push_back(SmartPoint(-1, -1, 0));
    points.push_back(SmartPoint(-3, 0, 1));
    points.push_back(SmartPoint(-5, 0, -1));
    points.push_back(SmartPoint(-50, 0, 1));
    points.push_back(SmartPoint(8, 0, -1));

    DivideAndConquerOpenMP daq(points);
    cout << daq << endl;
    daq.stl();
}

void Test::divideAndConquerOpenMP2() {
    vector<SmartPoint> points;
    points.push_back(SmartPoint(-1, 1, 0));
    points.push_back(SmartPoint(-1, -1, 0));
    points.push_back(SmartPoint(-1, .5, 1));
    points.push_back(SmartPoint(-1, -.5, 1));
    points.push_back(SmartPoint(-1, -.5, -1));
    points.push_back(SmartPoint(-1, .5, -1));
    points.push_back(SmartPoint(-10, 0, 0));

    points.push_back(SmartPoint(1, 0, 1));
    points.push_back(SmartPoint(1, 0, -1));
    points.push_back(SmartPoint(1, 1, 0));
    points.push_back(SmartPoint(1, -1, 0));
    points.push_back(SmartPoint(1.1, 0, 0));
    points.push_back(SmartPoint(.9, 0, .1));
    points.push_back(SmartPoint(.9, 0, -.1));

    random_shuffle(points.begin(), points.end());

    DivideAndConquerOpenMP dac(points);
    cout << dac << endl;
    dac.stl();
}

void Test::writeToAndReadFromArray() {

    vector<SmartPoint> points;
    points.push_back(SmartPoint(-1, 1, 0));
    points.push_back(SmartPoint(-1, -1, 0));
    points.push_back(SmartPoint(-1, .5, 1));
    points.push_back(SmartPoint(-1, -.5, 1));
    points.push_back(SmartPoint(-1, -.5, -1));
    points.push_back(SmartPoint(-1, .5, -1));
    points.push_back(SmartPoint(-10, 0, 0));

    points.push_back(SmartPoint(1, 0, 1));
    points.push_back(SmartPoint(1, 0, -1));
    points.push_back(SmartPoint(1, 1, 0));
    points.push_back(SmartPoint(1, -1, 0));
    points.push_back(SmartPoint(1.1, 0, 0));
    points.push_back(SmartPoint(.9, 0, .1));
    points.push_back(SmartPoint(.9, 0, -.1));

    random_shuffle(points.begin(), points.end());

    DivideAndConquerOpenMP dac(points);

    vector<double> vec;
    dac.writeToVector(vec);

    cout << endl;
    ConvexHull vecOut(vec);

    cout << vecOut;
}

void Test::divideAndConquerOpenMP3(int numPoints) {

    vector<SmartPoint> points;
    for (int i = 0; i < numPoints; i++)
        points.push_back(SmartPoint(10*(double)rand()/RAND_MAX, 10*(double)rand()/RAND_MAX, 10*(double)rand()/RAND_MAX));

    DivideAndConquerOpenMP dac(points);

    cout << dac;
    dac.stl();
}

void Test::incrementalMethod2(int numPoints) {

    vector<SmartPoint> points;
    for (int i = 0; i < numPoints; i++)
        points.push_back(SmartPoint(10*(double)rand()/RAND_MAX, 10*(double)rand()/RAND_MAX, 10*(double)rand()/RAND_MAX));

    IncrementalMethod im(points);

    cout << im;
    im.stl();
}
