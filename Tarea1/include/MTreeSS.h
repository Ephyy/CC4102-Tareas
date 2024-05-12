#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include "Cluster.h"

using namespace std;

class MTreeBySS {
    public:
        shared_ptr<Node> root;
        double max_size;
        double min_size;

        MTreeBySS(double max_size);

        // Function OutputLeafPage(
        // Cin: a set of points of cardinality no
        // greater that will fit into a disk page
        // )
        // Returns a tuple (m,r, a) where:
        //     m is the primary medoid of Cin,
        //     r is called the covering radius,
        //     a is the disk address of the page output.
        Entry outputLeafPage(Cluster c_in);

        // Function OutputInternalPage (
        // Cmra: a set of (m, r, a) tuples as returned from OutputLeafPage
        // )
        // Returns (M, R, A) where:
        //      M is the primary medoid of the set of points Cin = {m|∃(m,r, a) ∈ Cmra},
        //      R is called the covering radius,
        //      A is the disk address of the page output.
        Entry outputInternalPage(vector<Entry> c_mra);

        // Function BulkLoad(
        //      Cin: a set of points in a metric space,
        //      CMAX: maximum number of node entries that will fit into a disk page
        // )
        // Returns disk address of root page of constructed M-tree.
        shared_ptr<Node> bulkLoad(double max_size, vector<shared_ptr<Point>> points);

        void set_node(vector<shared_ptr<Point>> points);
};