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
    vector<Point> points = generateRandomPoints(100, 1);
    vector<shared_ptr<Point>> shared_points;
    for (Point p : points) {
        shared_points.push_back(make_shared<Point>(p));
    }
    points.clear();
    MTreeBySS mtree = MTreeBySS(10);

    cout << "Setting node..." << endl;
    mtree.set_node(shared_points);
    cout << "DONE" << endl;

    cout << "Final M tree" << endl;
    cout << "Entries Root Node: " << endl;
    for (Entry e : mtree.node->get_entries()) {
        cout << e.get_p() << endl;
    }
    cout << "Entries first son Node: " << endl;
    for (Entry e : mtree.node->get_entries()[0].get_a()->get_entries()) {
        cout << e.get_p() << endl;
    }
    cout << "Entries second son Node: " << endl;
    for (Entry e : mtree.node->get_entries()[1].get_a()->get_entries()) {
        cout << e.get_p() << endl;
    }
    // cout << "Entries third son Node: " << endl;
    // for (Entry e : mtree.node->get_entries()[2].get_a()->get_entries()) {
    //     cout << e.get_p() << endl;
    // }

    return 0;
}

