#include <iostream>
#include <memory>
#include "Point.h"
#include "Node.h"
#include "Entry.h"
#include "MTreeSS.h"
#include "RangeQuery.h"

using namespace std;

const int MAX = 4096;
const double RANGE = 0.02; // retorna aproximadamente un 0.12% de los puntos del conjunto

int main() {
    std::cout << "TESTING" << std::endl;

    // Set default values
    int entry_bytes = sizeof(Entry);	
    int max_entries = MAX / entry_bytes;

    // Create 100 queries
    vector<Point> query_points = generateRandomPoints(100, 1);
    vector<RangeQuery> queries;
    for (Point p : query_points) {
        queries.push_back(RangeQuery(p, RANGE));
    }

    // Create points for the M tree
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
    mtree.set_node(shared_points);
    cout << "M-Tree by SS algorithm finished." << endl;

    RangeQuery first_query = queries[0];
    vector<Point> answer;
    first_query.lookup(mtree.node, answer);
    cout << "First query answer: " << answer.size() << " points." << endl;

    return 0;
}

