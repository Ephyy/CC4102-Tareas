#ifndef POINT_H
#define POINT_H 

#include <math.h>
#include <iostream>



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

#endif