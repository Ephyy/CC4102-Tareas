#include <iostream>
#include <vector>
#include <utility>
#include "Point.h"

using namespace std;

class Cluster {
    public:
        // Ver si es mejor usar otra estructura de datos
        vector<Point> points;
        Point primary_medoid;
        double radius;
        // B 
        double max_size; 
        // b
        double min_size;

        Cluster(double max_size);

        // Obtains the primary medoid from the points in the cluster:
        // the least medoid of the cluster in this order.
        // 
        // A cluster medoid is any point m in the cluster for which there exists no other
        // point p in the cluster that, if nominated as a medoid, would give the cluster
        // a radius less than that given by m.
        Point set_primary_medoid();

        // The distance between two clusters is that between their primary medoids.
        double distance(Cluster cluster);

        void insert(Point p);

        int size();

        // Given a cluster c, a nearest neighbour of c in a set of clusters C is a cluster c'
        // such that there is no other cluster in C whose distance to c is less than
        // that between c and c'. (c may have multiple nearest neighbours in C.)
        pair<Cluster &, vector<Cluster>::iterator> nearest_neighbour(vector<Cluster> clusters);

        // Returns a NEW cluster that is the result of merging this cluster with another.
        Cluster &merge(Cluster &cluster);

        pair<Cluster &, Cluster &> split();
};

// Returns a pair of a pair with the cluster and its iterator from the closest pair of clusters.
// 
// A closest pair of clusters in C is a pair of clusters c1, c2 ∈ C such that
// d(c1, c2) <= d(ci, cj) for all ci, cj ∈ C
// 
// For the returned pair, |c1| > |c2|
pair<pair<Cluster &, vector<Cluster>::iterator>, pair<Cluster &, vector<Cluster>::iterator>> closest_pair(vector<Cluster> clusters);

// Returns a set of clusters, each of cardinality in [CMAX/2, CMAX].
// 
// Arguments:
//      CMAX: maximum acceptable cardinality of a cluster,
//      Cin: a set of at least CMAX/2 points
vector<Cluster> cluster(double CMAX, vector<Point> &c_in);
