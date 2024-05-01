#include "Point.h"
#include <cmath>

// Constructor
Point::Point() : x(0), y(0) {}
Point::Point(double x, double y) : x(x), y(y) {}

// Getters
double Point::get_x() {
    return x;
}

double Point::get_y() {
    return y;
}


double dist(Point p, Point q)
{   
    return sqrt(pow(p.get_x() - q.get_x(), 2) + pow(p.get_y() - q.get_y(), 2));
}
