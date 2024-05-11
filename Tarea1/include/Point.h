#ifndef POINT_H
#define POINT_H 

#include <math.h>
#include <vector>
#include <iostream>

using namespace std;


struct Point {
    double x;
    double y;
        
    // Constructor
    Point();
    Point(double x, double y);
    
    // Getters
    double get_x();
    double get_y();
};


double dist(Point p, Point q);
vector<Point> generateRandomPoints(int n, double range);
std::ostream& operator<<(std::ostream& os, const Point& p);
// Operator overloading
bool operator==(const Point& p1, const Point& p2);

#endif