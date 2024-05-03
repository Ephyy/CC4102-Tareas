#include "Cluster.h"
#include <algorithm>

using namespace std;



vector<Cluster> cluster(double max_size, vector<Point> &points) {
    // Let Cout = {} ;
    vector<Cluster> clusters_output;
    // Let C = {} ;
    vector<Cluster> clusters; 
    for_each(points.begin(), points.end(), [&clusters] (Point point) {
        Cluster singleton_cluster = Cluster();
        singleton_cluster.insert(point);
        clusters.push_back(singleton_cluster);
    });


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
    // Let c be the last remaining element of C
    Cluster last_cluster = clusters[0];
    Cluster neareast_cluster;
    if (clusters_output.size() > 0) {
        // Let c' be a nearest neighbour of c in Cout
        pair<Cluster &, vector<Cluster>::iterator> neareast_cluster_pair = last_cluster.nearest_neighbour(clusters_output);
        neareast_cluster = neareast_cluster_pair.first;
        vector<Cluster>::iterator neareast_cluster_iter = neareast_cluster_pair.second;
        // Remove c' from Cout
        clusters_output.erase(neareast_cluster_iter); // maybe neareast_cluster dissapears???
    } else {
        // Let c' = {};
        neareast_cluster = Cluster();
    }

    Cluster &merged_cluster = last_cluster.merge(neareast_cluster);
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