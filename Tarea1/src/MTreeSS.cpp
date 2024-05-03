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
        tuple<Point, double, vector<Entry>*> outputLeafPage(Cluster c_in){
            Point g = c_in.set_primary_medoid();
            double r = 0;
            vector<Entry> C;

            for (Point p : c_in.points) {
                C.push_back(Entry(p, NULL, NULL));
                r = max(r, dist(g, p));
            }

            vector<Entry>* a = &C;
            return make_tuple(g, r, a);
        }

        Entry outputLeafPage2(Cluster c_in){
            Point g = c_in.set_primary_medoid();
            double r = 0;
            Node C(max_size);

            for (Point p : c_in.points) {
                C.insert(Entry(p, NULL, NULL));
                r = max(r, dist(g, p));
            }

            std::shared_ptr<Node> a = &C;
            return Entry(g, r, a);
        }



        // Function OutputInternalPage (
        // Cmra: a set of (m,r, a) tuples as returned from OutputLeafPage
        // )
        // Returns (M, R, A) where:
        // M is the primary medoid of the set of
        // points Cin = {m|∃(m,r, a) ∈ Cmra},
        // R is called the covering radius,
        // A is the disk address of the page output.
        tuple<Point, double, vector<tuple<Point, double, vector<Entry>*>>*> outputInternalPage(vector<tuple<Point, double, vector<Entry>*>> c_mra){
            
            // We need to calculate the primary medoid of the set of points Cin
            Cluster C_in;
            for (tuple<Point, double, vector<Entry>*> t : c_mra) {
                C_in.points.push_back(get<0>(t));
            }
            Point G = C_in.set_primary_medoid();

            double R = 0;
            vector<tuple<Point, double, vector<Entry>*>> C;

            for (tuple<Point, double, vector<Entry>*> t : c_mra) {
                C.push_back(t);
                R = max(R, dist(G, get<0>(t)));
            }

            vector<tuple<Point, double, vector<Entry>*>>* A = &C;
            return make_tuple(G, R, A);
        }

        Entry outputInternalPage2(vector<Entry> c_mra){
            
            // We need to calculate the primary medoid of the set of points Cin
            Cluster C_in;
            for (Entry e : c_mra) {
                C_in.points.push_back(e.get_p());
            }
            Point G = C_in.set_primary_medoid();

            double R = 0;
            Node C(max_size);

            for (Entry e : c_mra) {
                C.insert(e);
                R = max(R, dist(G, e.get_p()));
            }

            std::shared_ptr<Node> A = std::make_shared<Node>(C);
            return Entry(G, R, A);
        }

        // Function BulkLoad(
        // Cin: a set of points in a metric space,
        // CMAX: maximum number of node entries that will fit into a disk page
        // )
        // Returns disk address of root page of constructed M-tree
        Node bulkLoad(vector<Point> &c_in);

        
};