#include <iostream>
#include <memory>
#include <chrono>

#include "Point.h"
#include "Node.h"
#include "Entry.h"
#include "MTreeSS.h"
#include "RangeQuery.h"

using namespace std;

const int MAX_BYTES = 4096;   
const double RANGE_QUERY = 0.02; // retorna aproximadamente un 0.12% de los puntos del conjunto

int main() {
    std::cout << "TESTING" << std::endl;

    // Set default values
    int entry_bytes = sizeof(Entry);	
    int max_entries = MAX_BYTES / entry_bytes;
    cout << "Max entries: " << max_entries << endl;

    // Create 100 queries
    vector<Point> query_points = generateRandomPoints(100, 1);
    vector<RangeQuery> queries;
    for (Point p : query_points) {
        queries.push_back(RangeQuery(p, RANGE_QUERY));
    }

    // Create points for the M treeS
    int exp = 10;
    int n = pow(2, exp);
    vector<Point> points = generateRandomPoints(n, 1);
    cout << "Generating M-Tree with " << n << " poins..." << endl;
    
    MTreeBySS mtree = MTreeBySS(max_entries);
    vector<shared_ptr<Point>> shared_points;
    for (Point p : points) {
        shared_points.push_back(make_shared<Point>(p));
    }
    points.clear();

    cout << "Setting node..." << endl;
    auto start = chrono::high_resolution_clock::now();
    mtree.set_node(shared_points);
    auto end = chrono::high_resolution_clock::now();
    cout << "M-Tree by SS algorithm finished." << endl;

    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Time: " << duration.count() << " ms" << endl;

    RangeQuery first_query = queries[0];
    vector<Point> answer;
    first_query.lookup(mtree.node, answer);
    cout << "First query answer: " << answer.size() << " points." << endl;

    return 0;
}

