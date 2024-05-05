#include <iostream>
#include <memory>
#include "Point.h"
#include "Node.h"
#include "Entry.h"
#include "MTreeSS.h"

using namespace std;

int main() {
    std::cout << "TESTING" << std::endl;

    Point p = Point(1, 2);
    auto n1 = std::make_shared<Node>(2);
    Entry e1 = Entry(p, 3, n1);

    std::cout << e1.get_a()->get_B() << std::endl;

    // MTREE BY SS TESTING
    vector<Point> points = generateRandomPoints(10, 10);
    cout << "Points: " << endl;

    for (Point p : points) {
        cout << p << endl;
    } 
    
    vector<shared_ptr<Point>> shared_points;
    for (Point p : points) {
        shared_points.push_back(make_shared<Point>(p));
    }

    MTreeBySS mtree = MTreeBySS(4);

    cout << "Setting node..." << endl;
    mtree.set_node(shared_points);
    cout << "DONE" << endl;

    cout << "Final M tree" << endl;
    cout << "Entries First Node: " << endl;
    int i = 0;
    for (Entry e : mtree.node->get_entries()) {
        int j = 0;
        cout << "NODO i:" << i << endl;
        cout << e.get_p() << endl;
        cout << "node i, j: " << i << j << endl;
        for (Entry e2 : e.get_a()->get_entries()) {
            cout << "node j:" << j << endl;
            cout << e2.get_p() << endl;
            j++;
        }
        i++;
    }
    return 0;
}

