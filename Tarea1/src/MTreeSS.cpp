#include "MTreeSS.h"

using namespace std;

MTreeBySS::MTreeBySS(double max_size) : max_size(max_size), min_size(max_size / 2) {
    this->node = nullptr;
}

// Returns an Entry where the point corresponds to the primary medoid of the given cluster and
// the radious is the maximum distance from the primary medoid to any point in the node to
// which it a points to.
Entry MTreeBySS::outputLeafPage(Cluster cluster_input) {
    Point medoide = *cluster_input.set_primary_medoid();
    double radius = 0;
    Node node_C(max_size);

    for (shared_ptr<Point> point_ptr : cluster_input.points) {
        node_C.insert(Entry(*point_ptr, 0, nullptr));
        radius = max(radius, dist(medoide, *point_ptr));
    }

    shared_ptr<Node> a = make_shared<Node>(node_C);
    return Entry(medoide, radius, a);
}

// entries_cmra son entradas que outputLeafPage ha devuelto en algún momento
// (es decir son entradas que apuntan a un nodo hoja)
// retorna una entrada que apunta a un nodo que contiene todas las entradas
// pedidas
Entry MTreeBySS::outputInternalPage(vector<Entry> entries_cmra) {
    // We need to calculate the primary medoid of the set of points Cin
    // we create a cluster with the points in entries_cmra
    Cluster cluster_input(max_size);
    for (Entry e : entries_cmra) {
        cluster_input.insert(make_shared<Point>(e.get_p()));
    }
    Point medoid = *cluster_input.set_primary_medoid();
    double radius = 0;
    Node interal_node_C(max_size);
    for (Entry e : entries_cmra) {
        interal_node_C.insert(e);
        radius = max(radius, dist(medoid, e.get_p()) + e.get_cr());
    }
    
    shared_ptr<Node> A = make_shared<Node>(interal_node_C);
    return Entry(medoid, radius, A);
}

shared_ptr<Node> MTreeBySS::bulkLoad(double max_size, vector<shared_ptr<Point>> points) {
    cout << "Bulk loading..." << endl;
    for (shared_ptr<Point> p : points) {
        cout << *p << endl;
    }
    // Todos los puntos caben en un mismo nodo
    if (points.size() <= max_size) {
        cout << "Points size is less than or equal to max size" << endl;
        Cluster cluster_input(max_size);
        for (shared_ptr<Point> p : points) {
            cluster_input.insert(p);
        }
        Entry entry = outputLeafPage(cluster_input);
        return entry.get_a();
    }

    // Clusterizo los puntos
    vector<Cluster> clusters_output = cluster_fun(max_size, points);
    cout << "Clusters output  DONE, size: " << clusters_output.size() << endl;
    for (Cluster c : clusters_output) {
        cout << "Cluster is: " << endl;
        for (shared_ptr<Point> p : c.points) {
            cout << *p << endl;
        }
    }

    // HASTA AQUI TODO BIEN EXCEPTO EL SPLIT POLICY!!!!!!!!-------------------------------------

    // Let C = {}, entries allocated
    vector<Entry> entries;
    // Por cada cluster, creo una entrada que apunta a un nodo que contiene los puntos del cluster
    for_each(clusters_output.begin(), clusters_output.end(), [&entries, this] (Cluster c) {
        // Add OutputLeafPage(c) to C
        entries.push_back(outputLeafPage(c));
    });

    // Mientras las entradas no quepan en un mismo nado, clusterizo los puntos de las entradas
    // y repito colocándolas en un nuevo nodo
    while (entries.size() > max_size) {
        // Let Cin = {m|(m, r, a) ∈ C}
        vector<shared_ptr<Point>> medoids_of_clusters;
        for (Entry e : entries) {
            medoids_of_clusters.push_back(make_shared<Point>(e.get_p()));
        }

        // Let Cout = Cluster(Cin, CMAX), clusterizo los medoides
        vector<Cluster> clusters_output = cluster_fun(max_size, medoids_of_clusters);

        // Let Cmra = {}
        vector<vector<Entry>> entries_for_each_cluster_Cmra;
        for_each(clusters_output.begin(), clusters_output.end(), [&entries_for_each_cluster_Cmra, &entries, this] (Cluster cluster) {
            // Let s = {(m, r, a)|(m, r, a) ∈ C (la entrada está en `entries`) ∧ m ∈ c (su punto está en `cluster`)} 
            // eso es, entradas correspondientes a este cluster
            vector<Entry> entries_by_cluster_s;
            for_each(cluster.points.begin(), cluster.points.end(), [&entries_by_cluster_s, &entries] (shared_ptr<Point> point) {
                auto it = find_if(entries.begin(), entries.end(), [&point] (Entry entry) {
                    return entry.get_p() == *point;
                });
                if (it != entries.end()) {
                    entries_by_cluster_s.push_back(*it);  // coloco la entrada, se copiará la entrada en el vector???
                }
            });
            entries_for_each_cluster_Cmra.push_back(entries_by_cluster_s);
        });

        // Let C = {} ;
        entries.clear();

        // For each s ∈ Cmra
        //     Add OutputInternalPage(s) to C ; 
        for_each(entries_for_each_cluster_Cmra.begin(), entries_for_each_cluster_Cmra.end(), [&entries, this] (vector<Entry> entries_s) {
            entries.push_back(outputInternalPage(entries_s));
        });
    }

    // Let (m, r, a) = OutputInternalPage(C)
    Entry final_entry = outputInternalPage(entries);
    return final_entry.get_a();
}

void MTreeBySS::set_node(vector<shared_ptr<Point>> points) {
    this->node = bulkLoad(max_size, points);
}  