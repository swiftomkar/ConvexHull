#pragma once
#include <iostream>
#include <vector>
#include <fstream>

/**
 * @author Dov Neimand
 * 
 * Describes a point in 3d space.
 */
class Point {
public:
    //Point();

    double x, y, z;
    static constexpr double EPSILON = 1e-7;

    Point() : x(0.0), y(0.0), z(0.0) {
    }

    Point(const Point& orig) : x(orig.x), y(orig.y), z(orig.z) {
    }

    Point(double x, double y, double z) : x(x), y(y), z(z) {
    }

    friend std::ostream& operator<<(std::ostream& s, const Point& p);
    double operator[](int i) const;
    double dot(const Point& p) const;
    Point cross(const Point& p) const;

    double distSquared(const Point& p)const;

    Point operator+(const Point& p) const;
    Point operator-(const Point& p) const;
    Point operator*(const double t) const;
    bool operator==(const Point& p) const;
    Point operator=(const Point& p);
    bool operator<(const Point &p) const;
    bool operator>(const Point &p) const;

    friend int colinear(const Point& a, const Point& b, const Point& c);
    double mag() const;
    double magSquared()const;
    double d(const Point& p)const;

    Point xyProj() const;

    friend std::ostream& operator<<(std::ostream& os, const Point& p);
    
    void stl(std::ofstream& out);

    void writeToVector(std::vector<double>& vec) const;
private:
    bool inBetween(const Point& a, const Point& b) const;
};

