#include <iostream>
#include <random>
#include <vector>
#include <cmath>

#include "Point.h"

using namespace std;

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

// Calcula la distancia entre dos puntos
// Ej: dist(Point(1, 2), Point(5, 3))
double dist(Point p, Point q)
{   
    return sqrt(pow(p.get_x() - q.get_x(), 2) + pow(p.get_y() - q.get_y(), 2));
}


// Generate n random points in the range [-range, range]
vector<Point> generateRandomPoints(int n, double range) {
    std::vector<Point> points;
    std::random_device rd; // Seed
    std::mt19937 gen(rd()); // Random number generator
    std::uniform_real_distribution<double> dis(-range, range);  

    for (int i = 0; i < n; i++) {
        double x = dis(gen);
        double y = dis(gen);
        Point p = Point(x, y);
        points.push_back(p);
    }

    return points;
}


// Overload << operator para imprimir puntos
std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}

