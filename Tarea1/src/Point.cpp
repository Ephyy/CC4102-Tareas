#include "Point.h"
#include <cmath>

// Constructor
Point::Point() : x(0), y(0) {}
Point::Point(double x, double y) : x(x), y(y) {}

// Getters
double Point::get_X() {
    return this->x;
}

double Point::get_Y() {
    return this->y;
}


double dist(Point p, Point q)
{   
    return sqrt(pow(p.get_X() - q.get_X(), 2) + pow(p.get_Y() - q.get_Y(), 2));
}
