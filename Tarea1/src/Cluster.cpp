#include <algorithm>
#include "Cluster.h"

using namespace std;

Cluster::Cluster(double max_size) : 
    max_size(max_size), 
    min_size(max_size / 2), 
    radius(0), 
    points() {
    this->primary_medoid = make_shared<Point>(Point(0, 0));
}

// Think about an optimization for this function 
// Podria ir guardando los iteradores de los puntos en un set
// Pensando que cada punto es una única instancia de Point, puedo guardar una tupla con ambos punteros
// y las distancias en un set ordenado por distancia ayyy no se
shared_ptr<Point> Cluster::set_primary_medoid() {
    if (this->points.size() == 1) {
        this->primary_medoid = shared_ptr<Point>(this->points[0]);
        cout << "Primary medoid set to: " << *this->primary_medoid << endl;
        return shared_ptr<Point>(this->primary_medoid);
    }
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
    cout << "Primary medoid set to: " << *this->primary_medoid << endl;
    return shared_ptr<Point>(this->primary_medoid);
}

double Cluster::distance(Cluster cluster) {
    // Assuming both clusters have set a primary medoid
    if (*this->primary_medoid == Point(0, 0) || *cluster.primary_medoid == Point(0, 0)) {
        cout << "Primary medoid not set" << endl;
    }
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

pair<Cluster &, vector<Cluster>::iterator> Cluster::nearest_neighbour(vector<Cluster> &clusters) {
    Cluster &nearest_cluster = clusters[0];
    auto nearest_cluster_iter = clusters.begin();
    double min_distance = this->distance(nearest_cluster);
    Point this_point = *this->primary_medoid;
    for (auto cluster_iter = clusters.begin(); cluster_iter != clusters.end(); cluster_iter++) {
        Cluster current_cluster = *cluster_iter;
        Point point = *current_cluster.primary_medoid;
        if (point == this_point) {
            continue;
        }
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
    merged_cluster.set_primary_medoid();
    return merged_cluster;
}

// MinMax split policy: Para cada par de puntos en el cluster:
// 
// 1. Escojo p1 y p2 como medoides de los clusters c1 y c2
// 2. Agrego el p más cercano a p1 y guardo su distancia
// 3. Agrego el p más cercano a p2 y guardo su distancia
// 4. Repito 2 y 3 hasta que se acaben los puntos del cluster, 
//    y el radio de cada cluster sería la distancía máxima 
//    calculada en el proceso
// 5. Guardo el radio mayor de los dos clusters
// 6. Repito 1 para cada par de puntos p1 y p2 y se escoge el par
//    con el radio menor
pair<Cluster, Cluster> Cluster::split() {
    // dado que creo los clusters adentro, dejo de existir cuando los retorno????? y con el merge?????
    Cluster cluster1 = Cluster(this->max_size);
    Cluster cluster2 = Cluster(this->max_size);
    // since I need to minimize the radius, I will start with the maximum possible radius
    double min_radius = this->radius; 
    for (auto iter1 = this->points.begin(); iter1 != this->points.end(); iter1++) {
        shared_ptr<Point> point1 = *iter1;
        for (auto iter2 = iter1 + 1; iter2 != this->points.end(); iter2++) {
            Cluster current_cluster1 = Cluster(this->max_size);
            Cluster current_cluster2 = Cluster(this->max_size);
            shared_ptr<Point> point2 = *iter2;
            cluster1.primary_medoid = shared_ptr<Point>(point1);
            cluster2.primary_medoid = shared_ptr<Point>(point2);
            for_each(this->points.begin(), this->points.end(), [&current_cluster1, &current_cluster2] (shared_ptr<Point> point) {
                double distance1 = dist(*point, *current_cluster1.primary_medoid);
                double distance2 = dist(*point, *current_cluster2.primary_medoid);
                if (distance1 < distance2) {
                    current_cluster1.insert(point);
                    current_cluster1.radius = max(current_cluster1.radius, distance1);
                } else {
                    current_cluster2.insert(point);
                    current_cluster2.radius = max(current_cluster1.radius, distance2);
                }
            });
            double max_radius = max(current_cluster1.radius, current_cluster2.radius);
            if (max_radius < min_radius) {
                min_radius = max_radius;
                cluster1 = current_cluster1;
                cluster2 = current_cluster2;
            }
        }
    }
    return make_pair(cluster1, cluster2);
}

pair<pair<Cluster &, vector<Cluster>::iterator>, pair<Cluster &, vector<Cluster>::iterator>> closest_pair(vector<Cluster> &clusters) {
    pair<Cluster &, vector<Cluster>::iterator> cluster1_pair = make_pair(ref(clusters[0]), clusters.begin());
    pair<Cluster &, vector<Cluster>::iterator> cluster2_pair = make_pair(ref(clusters[1]), clusters.begin() + 1);
    double min_distance = 100; //= cluster1_pair.first.distance(cluster2_pair.first);
    for (auto cluster1_iter = clusters.begin(); cluster1_iter != clusters.end(); cluster1_iter++) {
        Cluster current_cluster1 = *cluster1_iter;
        // for (auto cluster2_iter = cluster1_iter + 1; cluster2_iter != clusters.end(); cluster2_iter++) {
        //     Cluster &cluster2 = *cluster2_iter;
        //     double distance = cluster1.distance(cluster2);
        //     if (distance < min_distance) {
        //         min_distance = distance;
        //         cluster1_pair = make_pair(ref(cluster1), cluster1_iter);
        //         cluster2_pair = make_pair(ref(cluster2), cluster2_iter);
        //     }
        // }
        pair<Cluster &, vector<Cluster>::iterator> current_cluster2_pair = current_cluster1.nearest_neighbour(clusters);
        Cluster &current_cluster2 = current_cluster2_pair.first;
        double distance = current_cluster1.distance(current_cluster2);
        if (distance < min_distance) {
            min_distance = distance;
            cluster1_pair = make_pair(*cluster1_iter, cluster1_iter);
            cluster2_pair = current_cluster2_pair;
        }
    }
    if (cluster1_pair.first.size() < cluster2_pair.first.size()) {
        return make_pair(cluster2_pair, cluster1_pair);
    }
    return make_pair(cluster1_pair, cluster2_pair);
}

vector<Cluster> cluster_fun(double max_size, vector<shared_ptr<Point>> points) {
    cout << "Clustering..." << endl;
    for (shared_ptr<Point> p : points) {
        cout << *p << endl;
    }
    // First phase: converts the input set of points into a set of singleton clusters.
    // Let Cout = {} ;
    vector<Cluster> clusters_output;
    // Let C = {} ;
    vector<Cluster> clusters; 
    for_each(points.begin(), points.end(), [&clusters, max_size] (shared_ptr<Point> point) {
        Cluster singleton_cluster = Cluster(max_size);
        singleton_cluster.insert(point);
        singleton_cluster.set_primary_medoid();
        clusters.push_back(singleton_cluster);
    });

    cout << "Clusters: " << endl;
    for (Cluster c : clusters) {
        cout << "Cluster is: " << endl;
        for (shared_ptr<Point> p : c.points) {
            cout << *p << endl;
        }
    }

    cout << "Clusters size: " << clusters.size() << endl;

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
        cout << "Closest clusters: " << endl;
        cout << "Cluster 1: " << endl;
        cout << *cluster1.primary_medoid << endl;
        cout << "Cluster 2: " << endl;
        cout << *cluster2.primary_medoid << endl;
    
        if (cluster1.size() + cluster2.size() <= max_size) {
            Cluster merged_cluster = cluster1.merge(cluster2);
            cout << "Merged cluster: " << endl;
            for (shared_ptr<Point> p : merged_cluster.points) {
                cout << *p << endl;
            }
            cout << "Primary medoid of clusters before erased: " << endl;
            // aquí ya está repetido el primary medoid
            for (Cluster c : clusters) {
                cout << *c.primary_medoid << endl;
            }
            // Remove c1 and c2 from C
            clusters.erase(cluster1_iter);
            clusters.erase(cluster2_iter);
            cout << "(1) -- Primary medoid of clusters after erased: " << endl;
            for (Cluster c : clusters) {
                cout << *c.primary_medoid << endl;
            }
            // Add c1 ∪ c2 to C
            clusters.push_back(merged_cluster);
            cout << "(2) -- Primary medoid of clusters after erased AND push of merged:" << endl;
            for (Cluster c : clusters) {
                cout << *c.primary_medoid << endl;
            }

        } else {
            cout << "Merge is greater than max size" << endl;
            // Add c1 to Cout
            clusters_output.push_back(Cluster(cluster1)); // maybe cluster1 dissapears???
            // Remove c1 from C 
            clusters.erase(cluster1_iter);
            cout << "Primary medoid of clusters after erased: " << endl;
            for (Cluster c : clusters) {
                cout << *c.primary_medoid << endl;
            }
        }
        cout << "Clusters output are: " << endl;
        for (Cluster c : clusters_output) {
            cout << "Cluster out is: " << endl;
            for (shared_ptr<Point> p : c.points) {
                cout << *p << endl;
            }
        }
        cout << "General Clusters are: " << endl;
        for (Cluster c : clusters) {
            cout << "Genearl Cluster is: " << endl;
            for (shared_ptr<Point> p : c.points) {
                cout << *p << endl;
            }
        }
    }

    cout << "\n CLUSTERING DONE \n" << endl;
    cout << "Clusters output: " << endl;
    for (Cluster c : clusters_output) {
        cout << "Cluster is: " << endl;
        for (shared_ptr<Point> p : c.points) {
            cout << *p << endl;
        }
    }

    cout << "Clusters output size: " << clusters_output.size() << endl;
    cout << "Clusters size: " << clusters.size() << endl;

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
        // tal vez mejor hacer que reciba las referencias de los clusters ya creados y los modifique*****
        pair<Cluster, Cluster> split_clusters = merged_cluster.split();
        // Add c1 and c2 to Cout
        clusters_output.push_back(split_clusters.first);
        clusters_output.push_back(split_clusters.second);
    }
    return clusters_output;
}