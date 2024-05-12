#ifndef POINT_H
#define POINT_H 

#include <math.h>
#include <vector>
#include <iostream>
#include <compare>
#include <algorithm>

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

    // Operator overloading
    bool operator==(const Point &p) const;
};


double dist(Point p, Point q);
vector<Point> generateRandomPoints(int n, double range);
vector<Point> generateRandomPoints(int n, int seed);
std::ostream& operator<<(std::ostream& os, const Point& p);

// Closest pair of points
bool compareX(Point p1, Point p2);
bool compareY(Point p1, Point p2);
pair<double, pair<Point, Point>> bruteForce(vector<Point> P, int n);
pair<double, pair<Point, Point>> stripClosest(vector<Point> strip, int size, double d, pair<Point, Point> closest);
pair<double, pair<Point, Point>> closestUtil(vector<Point> Px, vector<Point> Py, int n);
pair<double, pair<Point, Point>> closest(vector<Point> points, int n);




#endif