#ifndef POINT_H
#define POINT_H 

#include <math.h>
#include <iostream>



class Point {
private:
    double x;
    double y;
public:
        
    // Constructor
    Point();
    Point(double x, double y);
    
    // Getters
    double get_X();
    double get_Y();
};


double dist(Point p, Point q);

#endif