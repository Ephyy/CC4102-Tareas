#include <math.h> 
#include <iostream>

using namespace std;

class Point {
    public:
        double x;
        double y;
        
        Point(double x, double y) {
            this->x = x;
            this->y = y;
        }

        double distance(Point p) {
            return sqrt((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y));
        }
};