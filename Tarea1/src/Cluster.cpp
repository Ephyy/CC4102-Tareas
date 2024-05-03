#include <iostream>
#include <vector>
#include "Point.h"

using namespace std;

class Cluster {
    public:
        vector<Point> points;
        Point primary_medoid;
        double radius;
        double max_size;
        double min_size;

        Cluster() {
            radius = 0;
            max_size = 0;
            min_size = 0;
        }
        
};