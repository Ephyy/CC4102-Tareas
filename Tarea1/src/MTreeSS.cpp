#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include "Node.h"
#include "Cluster.h"

using namespace std;

class MTreeBySS {
    public:
        Node &root;
        double max_size;
        double min_size;

        MTreeBySS(double max_size, double min_size, Node &root) : root(root) {
            this->max_size = max_size;
            this->min_size = min_size;
            // root = new MTreeNode();
        }

        // Function OutputLeafPage(
        // Cin: a set of points of cardinality no
        // greater that will fit into a disk page
        // )
        // Returns a tuple (m,r, a) where:
        //     m is the primary medoid of Cin,
        //     r is called the covering radius,
        //     a is the disk address of the page output.
        Entry outputLeafPage(Cluster c_in) {
            Point g = *c_in.set_primary_medoid();
            double r = 0;
            Node C(max_size);

            for (shared_ptr<Point> p : c_in.points) {
                C.insert(Entry(*p, 0, nullptr));
                r = max(r, dist(g, *p));
            }

            shared_ptr<Node> a = make_shared<Node>(C);
            return Entry(g, r, a);
        }

        // Function OutputInternalPage (
        // Cmra: a set of (m, r, a) tuples as returned from OutputLeafPage
        // )
        // Returns (M, R, A) where:
        //      M is the primary medoid of the set of points Cin = {m|∃(m,r, a) ∈ Cmra},
        //      R is called the covering radius,
        //      A is the disk address of the page output.
        Entry outputInternalPage(vector<Entry> c_mra) {
            // We need to calculate the primary medoid of the set of points Cin
            Cluster C_in(max_size);
            for (Entry e : c_mra) {
                C_in.points.push_back(make_shared<Point>(e.get_p()));
            }
            Point G = *C_in.set_primary_medoid();

            double R = 0;
            Node C(max_size);

            for (Entry e : c_mra) {
                C.insert(e);
                R = max(R, dist(G, e.get_p()));
            }
            
            shared_ptr<Node> A = make_shared<Node>(C);
            return Entry(G, R, A);
        }

        // Function BulkLoad(
        //      Cin: a set of points in a metric space,
        //      CMAX: maximum number of node entries that will fit into a disk page
        // )
        // Returns disk address of root page of constructed M-tree.
        shared_ptr<Node> bulkLoad(double max_size, vector<shared_ptr<Point>> points) {
            if (points.size() <= max_size) {
                Cluster c_in(max_size);
                for (shared_ptr<Point> p : points) {
                    c_in.insert(p);
                }
                Entry entry = outputLeafPage(c_in);
                return entry.get_a();
            }

            vector<Cluster> clusters_output = cluster_fun(max_size, points);
            // Let C = {}, entries allocated
            vector<Entry> entries;
            for_each(clusters_output.begin(), clusters_output.end(), [&entries, this] (Cluster c) {
                // Add OutputLeafPage(c) to C
                entries.push_back(outputLeafPage(c));
            });
            while (entries.size() > max_size) {
                // Let Cin = {m|(m, r,a) ∈ C}
                vector<shared_ptr<Point>> new_points;
                for (Entry e : entries) {
                    new_points.push_back(make_shared<Point>(e.get_p())); // make_shared llama a uno nuevo
                }
                // Let Cout = Cluster(Cin, CMAX)
                vector<Cluster> clusters_output = cluster_fun(max_size, new_points);
                // Let Cmra = {}
                vector<Entry> entries_mra;
                for_each(clusters_output.begin(), clusters_output.end(), [&entries_mra, &entries, this] (Cluster cluster) {
                    // Let s = {(m, r, a)|(m, r, a) ∈ C ∧ m ∈ c} 
                    // todas las entradas que están en entries (C) y que su punto está en el cluster c
                    for_each(entries.begin(), entries.end(), [&entries_mra, &cluster] (Entry entry) {
                        auto it = find_if(cluster.points.begin(), cluster.points.end(), [&entry] (shared_ptr<Point> point) {
                            return entry.get_p() == *point;
                        });
                        if (it != cluster.points.end()) {
                            entries_mra.push_back(entry);
                        }
                    });
                });
                
                // vector<Entry> new_entries;
                // new_entries.push_back(outputInternalPage(entries_mra));
                // Let C = {} ;
                entries.clear();
                // For each s ∈ Cmra
                //     Add OutputInternalPage(s) to C ; --> no entiendo bien a que se refiere???
                entries.push_back(outputInternalPage(entries_mra));
            }
            // Let (m, r, a) = OutputInternalPage(C)
            Entry entry = outputInternalPage(entries);
            return entry.get_a();
        }
        
};