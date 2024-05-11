#include "RangeQuery.h"

using namespace std;

RangeQuery::RangeQuery(Point q, double r) : q(q), r(r) {}

void RangeQuery::lookup(shared_ptr<Node> node, vector<Point> &answer) {
    if (node->is_leaf()) {
        for (Entry entry : node->get_entries()) {
            if (dist(entry.get_p(), q) <= r) {
                answer.push_back(Point(entry.get_p()));
            }
        }
    } else {
        for (Entry entry : node->get_entries()) {
            if (dist(entry.get_p(), q) <= r + entry.get_cr()) {
                lookup(entry.get_a(), answer);
            }
        }
    }
    return;
}
