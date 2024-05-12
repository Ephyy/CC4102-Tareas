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

bool Point::operator==(const Point &p) const {
    return x == p.x && y == p.y;
}

// Calcula la distancia entre dos puntos
// Ej: dist(Point(1, 2), Point(5, 3))
double dist(Point p, Point q)
{   
    return sqrt(pow(p.get_x() - q.get_x(), 2) + pow(p.get_y() - q.get_y(), 2));
}


// Generate n random points in the range [0, range]
vector<Point> generateRandomPoints(int n, double range) {
    std::vector<Point> points;
    std::random_device rd; // Seed
    std::mt19937 gen(rd()); // Random number generator
    std::uniform_real_distribution<double> dis(0, range);  

    for (int i = 0; i < n; i++) {
        double x = dis(gen);
        double y = dis(gen);
        Point p = Point(x, y);
        points.push_back(p);
    }

    return points;
}

// Generate n random points in the range [0, 1]
vector<Point> generateRandomPoints(int n, int seed) {
    std::vector<Point> points;
    std::mt19937 gen(seed); // Random number generator
    std::uniform_real_distribution<double> dis(0, 1);  // Rango [0, 1]

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

// Needed to sort array of points according to X coordinate
bool compareX(Point p1, Point p2) {
    // Point *p1 = (Point *)a,  *p2 = (Point *)b;
    return (p1.x != p2.x) ? (p1.x < p2.x) : (p1.y < p2.y);
}

// Needed to sort array of points according to Y coordinate
bool compareY(Point p1, Point p2) {
    // Point *p1 = (Point *)a,   *p2 = (Point *)b;
    return (p1.y != p2.y) ? (p1.y < p2.y) : (p1.x < p2.x);
}
 
// A Brute Force method to return the smallest distance between two points
// in P[] of size n
pair<double, pair<Point, Point>> bruteForce(vector<Point> P, int n) {
    double min = 100;
    pair<Point, Point> closest_pair;
    for (int i = 0; i < n; ++i) {
        for (int j = i+1; j < n; ++j) {
            if (dist(P[i], P[j]) < min) {
                closest_pair = make_pair(Point(P[i]), Point(P[j]));
                min = dist(P[i], P[j]);
            }
        }
    }
    return make_pair(min, closest_pair);
}

// A utility function to find the distance between the closest points of
// strip of a given size. All points in strip[] are sorted according to
// y coordinate. They all have an upper bound on minimum distance as d.
// Note that this method seems to be a O(n^2) method, but it's a O(n)
// method as the inner loop runs at most 6 times
pair<double, pair<Point, Point>> stripClosest(vector<Point> strip, int size, double d, pair<Point, Point> closest) {
    double min = d;  // Initialize the minimum distance as d
    pair<Point, Point> closest_pair = closest;
 
    // Pick all points one by one and try the next points till the difference
    // between y coordinates is smaller than d.
    // This is a proven fact that this loop runs at most 6 times
    for (int i = 0; i < size; ++i) {
        for (int j = i+1; j < size && (strip[j].y - strip[i].y) < min; ++j) {
            if (dist(strip[i],strip[j]) < min) {
                closest_pair = make_pair(Point(strip[i]), Point(strip[j]));
                min = dist(strip[i], strip[j]);
            }
        }
    }
    return make_pair(min, closest_pair);
}
 
// A recursive function to find the smallest distance. The array Px contains
// all points sorted according to x coordinates and Py contains all points
// sorted according to y coordinates
pair<double, pair<Point, Point>> closestUtil(vector<Point> Px, vector<Point> Py, int n) {
    // If there are 2 or 3 points, then use brute force
    if (n <= 3)
        return bruteForce(Px, n);
 
    // Find the middle point
    int mid = n/2;
    Point midPoint = Px[mid];
 
    // Divide points in y sorted array around the vertical line.
    // Assumption: All x coordinates are distinct.
    vector<Point> Pyl(mid);  // y sorted points on right of vertical line 
    vector<Point> Pyr(n-mid);  // y sorted points on right of vertical line
    int li = 0, ri = 0;  // indexes of left and right subarrays
    for (int i = 0; i < n; i++) {
      if ((Py[i].x < midPoint.x || (Py[i].x == midPoint.x && Py[i].y < midPoint.y)) && li < mid)
         Pyl[li++] = Py[i];
      else
         Pyr[ri++] = Py[i];
    }
    vector<Point> new_Px(Px.begin() + mid, Px.end());
 
    // Consider the vertical line passing through the middle point
    // calculate the smallest distance dl on left of middle point and
    // dr on right side
    pair<double, pair<Point, Point>> closest_l = closestUtil(Px, Pyl, mid);
    pair<double, pair<Point, Point>> closest_r = closestUtil(new_Px, Pyr, n-mid);

    double dl = closest_l.first;
    double dr = closest_r.first;
 
    // Find the smaller of two distances
    // double d = min(dl, dr);
    double d;
    pair<Point, Point> closest;
    if (dr <= dl) {
        d = dr;
        closest = closest_r.second;
    } else {
        d = dl;
        closest = closest_l.second;
    }
 
    // Build an array strip[] that contains points close (closer than d)
    // to the line passing through the middle point
    // Point strip[n];
    vector<Point> strip(n);
    int j = 0;
    for (int i = 0; i < n; i++) {
        if (abs(Py[i].x - midPoint.x) < d)
            strip[j] = Py[i], j++;
 
    }
    // Find the closest points in strip.  Return the minimum of d and closest
    // distance is strip[]
    return stripClosest(strip, j, d, closest);
}
 
// The main function that finds the smallest distance
// This method mainly uses closestUtil()
pair<double, pair<Point, Point>> closest(vector<Point> points, int n) {
    // Point Px[n];
    vector<Point> Px(points);
    // Point Py[n];
    vector<Point> Py(points);
 
    sort(Px.begin(), Px.end(), compareX);
    sort(Py.begin(), Py.end(), compareY);
    
    // Use recursive function closestUtil() to find the smallest distance
    return closestUtil(Px, Py, n);
}
