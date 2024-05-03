#include "Cluster.h"
#include <algorithm>

using namespace std;

// Cluster::Cluster(double max_size) {
//     this->max_size = max_size;
//     this->min_size = max_size / 2;
//     this->radius = 0;
//     this->points = ();
//     // Point medoid = this->points[0]; // it this even ok??? is it garbage???
//     this->primary_medoid = Point(); // gets a reference to the garbage???
// }
Cluster::Cluster(double max_size) : 
    max_size(max_size), 
    min_size(max_size / 2), 
    radius(0), 
    points() {
}

// Think about an optimization for this function 
// Podria ir guardando los iteradores de los puntos en un set
// Pensando que cada punto es una única instancia de Point, puedo guardar una tupla con ambos punteros
// y las distancias en un set ordenado por distancia ayyy no se
Point Cluster::set_primary_medoid() {
    if (this->points.size() == 0) {
        return Point();
    }
    // double medoid_radius = this->radius;
    // Point medoid;
    for_each(this->points.begin(), this->points.end(), [this] (shared_ptr<Point> point) {
        double max_radius = 0;
        for_each(this->points.begin(), this->points.end(), [&max_radius, point] (shared_ptr<Point> other_point) {
            max_radius = max(max_radius, dist(*point, *other_point));
        });
        if (max_radius > this->radius) {
            this->radius = max_radius;
            this->primary_medoid = shared_ptr<Point>(point);
        }
    });
    return *this->primary_medoid;
}

double Cluster::distance(Cluster cluster) {
    // Assuming both clusters have set a primary medoid
    return dist(*this->primary_medoid, *(cluster.primary_medoid));
}

// Test proper insertion of points *******************
void Cluster::insert(shared_ptr<Point> point) {
    shared_ptr<Point> point_ptr = shared_ptr<Point>(point);
    this->points.push_back(point_ptr);
}

int Cluster::size() {
    return this->points.size();
}

pair<Cluster &, vector<Cluster>::iterator> Cluster::nearest_neighbour(vector<Cluster> clusters) {
    Cluster &nearest_cluster = clusters[0];
    vector<Cluster>::iterator nearest_cluster_iter = clusters.begin();
    double min_distance = this->distance(nearest_cluster);
    for (vector<Cluster>::iterator cluster_iter = clusters.begin(); cluster_iter != clusters.end(); cluster_iter++) {
        Cluster &current_cluster = *cluster_iter;
        double distance = this->distance(current_cluster);
        if (distance < min_distance) {
            min_distance = distance;
            nearest_cluster = current_cluster;
            nearest_cluster_iter = cluster_iter;
        }
    }
    return make_pair(ref(nearest_cluster), nearest_cluster_iter);
}

Cluster Cluster::merge(Cluster &cluster) {
    Cluster merged_cluster = Cluster(this->max_size);
    for_each(this->points.begin(), this->points.end(), [&merged_cluster] (shared_ptr<Point> point) {
        merged_cluster.insert(shared_ptr<Point>(point));
    });
    for_each(cluster.points.begin(), cluster.points.end(), [&merged_cluster] (shared_ptr<Point> point) {
        merged_cluster.insert(shared_ptr<Point>(point));
    });
    return merged_cluster;
}

vector<Cluster> cluster(double max_size, vector<shared_ptr<Point>> points) {
    // First phase: converts the input set of points into a set of singleton clusters.
    // Let Cout = {} ;
    vector<Cluster> clusters_output;
    // Let C = {} ;
    vector<Cluster> clusters; 
    for_each(points.begin(), points.end(), [&clusters, max_size] (shared_ptr<Point> point) {
        Cluster singleton_cluster = Cluster(max_size);
        singleton_cluster.insert(point);
        clusters.push_back(singleton_cluster);
    });

    // Second phase: work of clustering.
    while (clusters.size() > 1) {
        pair<pair<Cluster &, vector<Cluster>::iterator>, pair<Cluster &, vector<Cluster>::iterator>> closest_clusters = closest_pair(clusters);
        //  Get the values of the cluster and its iterator from the closest pair of clusters.
        pair<Cluster &, vector<Cluster>::iterator> cluster1_pair = closest_clusters.first;
        Cluster &cluster1 = cluster1_pair.first;
        vector<Cluster>::iterator cluster1_iter = cluster1_pair.second;
        pair<Cluster &, vector<Cluster>::iterator> cluster2_pair = closest_clusters.second;
        Cluster &cluster2 = cluster2_pair.first;
        vector<Cluster>::iterator cluster2_iter = cluster2_pair.second;
        if (cluster1.size() + cluster2.size() <= max_size) {
            Cluster merged_cluster = cluster1.merge(cluster2);
            // Remove c1 and c2 from C
            clusters.erase(cluster1_iter);
            clusters.erase(cluster2_iter);
            // Add c1 ∪ c2 to C
            clusters.push_back(merged_cluster);
        } else {
            // Add c1 to Cout
            clusters_output.push_back(cluster1); // maybe cluster1 dissapears???
            // Remove c1 from C 
            clusters.erase(cluster1_iter);
        }
    }

    // Third phase: if the last remeining element of C contains fewer than CMAX/2 points, its points and those
    // of its nearest neighbour are reditributed to ensure that no cluster breaks the minimum size bound.
    Cluster last_cluster = clusters[0];
    Cluster neareast_cluster = Cluster(max_size);
    if (clusters_output.size() > 0) {
        // Let c' be a nearest neighbour of c in Cout
        pair<Cluster &, vector<Cluster>::iterator> neareast_cluster_pair = last_cluster.nearest_neighbour(clusters_output);
        neareast_cluster = neareast_cluster_pair.first;
        vector<Cluster>::iterator neareast_cluster_iter = neareast_cluster_pair.second;
        // Remove c' from Cout
        clusters_output.erase(neareast_cluster_iter); // maybe neareast_cluster dissapears???
    } 
    // else {
    //     // Let c' = {};
    //     neareast_cluster = Cluster(max_size);
    // }

    Cluster merged_cluster = last_cluster.merge(neareast_cluster);
    if (merged_cluster.size() <= max_size) {
        // Add c ∪ c' to Cout 
        clusters_output.push_back(merged_cluster);
    } else {
        // Split c ∪ c' into c1 and c2 using the insertion splitting policy
        pair<Cluster &, Cluster &> split_clusters = merged_cluster.split();
        // Add c1 and c2 to Cout
        clusters_output.push_back(split_clusters.first);
        clusters_output.push_back(split_clusters.second);
    }

    return clusters_output;
}