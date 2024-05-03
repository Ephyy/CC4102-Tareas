#include "Cluster.h"
#include <algorithm>

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
        pair<Cluster &, Cluster &> closest_pos = closest_pair(clusters);
        Cluster &cluster1 = closest_pos.first;
        Cluster &cluster2 = closest_pos.second;
        if (cluster1.size() + cluster2.size() <= max_size) {
            Cluster merged_cluster = cluster1.merge(cluster2);
            clusters.erase(find(clusters.begin(), clusters.end(), cluster1));
            clusters.erase(find(clusters.begin(), clusters.end(), cluster2));
            clusters.push_back(merged_cluster);
        } else {
            clusters_output.push_back(cluster1); // maybe cluster1 dissapears???
            clusters.erase(find(clusters.begin(), clusters.end(), cluster1));
        }
    }
    
    Cluster last_cluster = clusters[0];
    Cluster neareast_cluster;
    if (clusters_output.size() > 0) {
        neareast_cluster = last_cluster.nearest_neighbour(clusters_output);
        clusters_output.erase(find(clusters_output.begin(), clusters_output.end(), neareast_cluster));
    } else {
        neareast_cluster = Cluster();
    }

    Cluster &merged_cluster = last_cluster.merge(neareast_cluster);
    if (last_cluster.size() + neareast_cluster.size() <= max_size) {
        clusters_output.push_back(merged_cluster);
    } else {
        pair<Cluster &, Cluster &> split_clusters = merged_cluster.split();
        clusters_output.push_back(split_clusters.first);
        clusters_output.push_back(split_clusters.second);
    }
    return clusters_output;
}