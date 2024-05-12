#include <algorithm>
#include "Cluster.h"

using namespace std;

Cluster::Cluster(double max_size) : 
    max_size(max_size), 
    min_size(max_size / 2), 
    radius(0), 
    points() {
}

shared_ptr<Point> Cluster::set_primary_medoid() {
    if (this->points.size() == 1) {
        this->primary_medoid = shared_ptr<Point>(this->points[0]);
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
    return shared_ptr<Point>(this->primary_medoid);
}

double Cluster::distance(Cluster cluster) {
    // Assuming both clusters have set a primary medoid
    return dist(*this->primary_medoid, *(cluster.primary_medoid));
}

void Cluster::insert(shared_ptr<Point> point) {
    shared_ptr<Point> point_ptr = shared_ptr<Point>(point);
    this->points.push_back(point_ptr);
}

int Cluster::size() {
    return this->points.size();
}

pair<Cluster, vector<Cluster>::iterator> Cluster::nearest_neighbour(vector<Cluster> &clusters) {
    Cluster nearest_cluster = *this;
    Point this_point = *this->primary_medoid;
    vector<Cluster>::iterator nearest_cluster_iter;
    double min_distance = 100; // dado que se minimiza, parto con un maximo
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
    return make_pair(nearest_cluster, nearest_cluster_iter);
}

Cluster Cluster::merge(Cluster cluster) {
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
// ----------------
// Varios puntos:
// -elijo dos
// le agrego el más cercano p1
// le agrego el más cercano p2
// no debería ser más de 2B 
pair<Cluster, Cluster> Cluster::split() {
    // dado que creo los clusters adentro, dejo de existir cuando los retorno????? y con el merge?????
    // since I need to minimize the radius, I will start with the maximum possible radius
    Cluster final_cluster1 = Cluster(this->max_size);
    Cluster final_cluster2 = Cluster(this->max_size);
    double min_radius = 100; 
    for (auto iter1 = this->points.begin(); iter1 != this->points.end(); iter1++) {
        Cluster cluster1 = Cluster(this->max_size);
        shared_ptr<Point> medoid1_ptr = shared_ptr<Point>(*iter1); // obtengo su medoide
        cluster1.insert(medoid1_ptr); // lo inserto
        cluster1.primary_medoid = medoid1_ptr; // lo seteo
        vector<pair<double, shared_ptr<Point>>> distances1;
        for_each(this->points.begin(), this->points.end(), [&distances1, &cluster1] (shared_ptr<Point> point) {
            Point point_to_compare = *point;
            if (point_to_compare == *cluster1.primary_medoid) {
                return;
            }
            double distance = dist(*cluster1.primary_medoid, point_to_compare);
            distances1.push_back(make_pair(distance, shared_ptr<Point>(point)));
        });
        sort(distances1.begin(), distances1.end());
        for (auto iter2 = iter1 + 1; iter2 != this->points.end(); iter2++) {
            Cluster cluster1_copy = Cluster(cluster1);
            vector<pair<double, shared_ptr<Point>>> distances1_copy(distances1);
            Cluster cluster2 = Cluster(this->max_size);
            shared_ptr<Point> medoid2_ptr = shared_ptr<Point>(*iter2);
            cluster2.insert(medoid2_ptr);
            cluster2.primary_medoid = medoid2_ptr;
            vector<pair<double, shared_ptr<Point>>> distances2;
            for_each(this->points.begin(), this->points.end(), [&distances2, &cluster2] (shared_ptr<Point> point) {
                Point point_to_compare = *point;
                if (point_to_compare == *cluster2.primary_medoid) {
                    return;
                }
                double distance = dist(*cluster2.primary_medoid, point_to_compare);
                distances2.push_back(make_pair(distance, shared_ptr<Point>(point)));
            });
            sort(distances2.begin(), distances2.end());
            // borro en cada uno el correspondiente al otro
            distances1_copy.erase(find_if(distances1_copy.begin(), distances1_copy.end(), [&medoid2_ptr] (pair<double, shared_ptr<Point>> p) {
                return *p.second == *medoid2_ptr;
            }));
            distances2.erase(find_if(distances2.begin(), distances2.end(), [&medoid1_ptr] (pair<double, shared_ptr<Point>> p) {
                return *p.second == *medoid1_ptr;
            }));

            // Alterno agregando los puntos más cercanos a cada medoide
            while (distances1_copy.size() > 0 && distances2.size() > 0) {
                // Agrego el más cercano a medoid1
                shared_ptr<Point> closest_to_medoid1 = distances1_copy[0].second;
                // Actualizo el radio de cluster1_copy
                if (distances1_copy[0].first > cluster1_copy.radius) {
                    cluster1_copy.radius = distances1_copy[0].first;
                }
                cluster1_copy.insert(closest_to_medoid1);
                // Borro el que inserté
                distances1_copy.erase(distances1_copy.begin()); 
                distances2.erase(find_if(distances2.begin(), distances2.end(), [&closest_to_medoid1] (pair<double, shared_ptr<Point>> p) {
                    return *p.second == *closest_to_medoid1;
                }));
                if (distances1_copy.size() == 0 || distances2.size() == 0) {
                    break;
                }
            
                // Agrego el más cercano a medoid2
                shared_ptr<Point> closest_to_medoid2 = distances2[0].second;
                // Actualizo el radio de cluster2
                if (distances2[0].first > cluster2.radius) {
                    cluster2.radius = distances2[0].first;
                }
                cluster2.insert(closest_to_medoid2);
                // Borro el que inserté
                distances2.erase(distances2.begin());
                distances1_copy.erase(find_if(distances1_copy.begin(), distances1_copy.end(), [&closest_to_medoid2] (pair<double, shared_ptr<Point>> p) {
                    return *p.second == *closest_to_medoid2;
                }));
            };

            // Tengo la división de los puntos en dos clusters y sus radios
            if (max(cluster1_copy.radius, cluster2.radius) < min_radius) {
                min_radius = max(cluster1_copy.radius, cluster2.radius);
                final_cluster1 = cluster1_copy;
                final_cluster2 = cluster2;
            }
        }
    }
    return make_pair(final_cluster1, final_cluster2);
}

bool Cluster::operator==(const Cluster &cluster) const {
    return this->primary_medoid == cluster.primary_medoid;
}

pair<pair<Cluster, vector<Cluster>::iterator>, pair<Cluster, vector<Cluster>::iterator>> closest_pair(vector<Cluster> &clusters) {
    pair<Cluster, vector<Cluster>::iterator> cluster1_pair = make_pair(clusters[0], clusters.begin());
    pair<Cluster, vector<Cluster>::iterator> cluster2_pair = make_pair(clusters[1], clusters.begin() + 1);
    vector<Point> points;
    for (auto cluster : clusters) { // en tiempo n
        points.push_back(Point(*cluster.primary_medoid));
    }
    pair<double, pair<Point, Point>> closest_points = closest(points, points.size()); // tiempo nlogn

    Point p1 = closest_points.second.first;
    Point p2 = closest_points.second.second;
    vector<Cluster>::iterator cluster1_iter = find_if(clusters.begin(), clusters.end(), [&p1] (Cluster c) { // tiempo n
        return *c.primary_medoid == p1;
    });
    vector<Cluster>::iterator cluster2_iter = find_if(clusters.begin(), clusters.end(), [&p2] (Cluster c) {
        return *c.primary_medoid == p2;
    });

    cluster1_pair = make_pair(Cluster(*cluster1_iter), cluster1_iter);
    cluster2_pair = make_pair(Cluster(*cluster2_iter), cluster2_iter);

    return make_pair(cluster1_pair, cluster2_pair);
}

vector<Cluster> cluster_fun(double max_size, vector<shared_ptr<Point>> points) {
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

    // Second phase: work of clustering.
    while (clusters.size() > 1) {
        pair<pair<Cluster, vector<Cluster>::iterator>, pair<Cluster, vector<Cluster>::iterator>> closest_clusters = closest_pair(clusters);

        //  Get the values of the cluster and its iterator from the closest pair of clusters.
        pair<Cluster, vector<Cluster>::iterator> cluster1_pair = closest_clusters.first;
        Cluster cluster1 = cluster1_pair.first;
        vector<Cluster>::iterator cluster1_iter = cluster1_pair.second;
        pair<Cluster, vector<Cluster>::iterator> cluster2_pair = closest_clusters.second;
        Cluster cluster2 = cluster2_pair.first;
        vector<Cluster>::iterator cluster2_iter = cluster2_pair.second;

        if (cluster1.size() + cluster2.size() <= max_size) {
            Cluster merged_cluster = cluster1.merge(cluster2);

            // Remove c1 and c2 from C
            clusters.erase(cluster1_iter); 
            auto cluster2_iter = find(clusters.begin(), clusters.end(), cluster2);
            clusters.erase(cluster2_iter);

            // Add c1 ∪ c2 to C
            clusters.push_back(merged_cluster);
        } else {
            // Add c1 to Cout: el c1, que era el mayor de ambos, está listo para ser añadido
            clusters_output.push_back(Cluster(cluster1)); 

            // Remove c1 from C 
            clusters.erase(cluster1_iter);
        }
    }

    // Third phase: if the last remeining element of C (clusters) contains fewer than CMAX/2 points, 
    // its points and those of its nearest neighbour are reditributed to ensure that no cluster 
    // breaks the minimum size bound.
    Cluster last_cluster = clusters[0];
    Cluster neareast_cluster = Cluster(max_size);
    if (clusters_output.size() > 0) {
        // Let c' be a nearest neighbour of c in Cout
        pair<Cluster, vector<Cluster>::iterator> neareast_cluster_pair = last_cluster.nearest_neighbour(clusters_output);
        neareast_cluster = neareast_cluster_pair.first;
        vector<Cluster>::iterator neareast_cluster_iter = neareast_cluster_pair.second;

        // Remove c' from Cout
        clusters_output.erase(neareast_cluster_iter); // maybe neareast_cluster dissapears???
    } 

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