#include <iostream>
#include <algorithm>
#include "Point.h"
#include "Entry.h"
#include "Node.h"

using namespace std;

class RangeQuery {
    public:
        Point q;
        // Radio de búsqueda
        double r;

        RangeQuery(Point q, double r);

        // Encuentra todos los puntos del M-Tree que residen
        // dentro de la bola
        int lookup(shared_ptr<Node> node, vector<Point> &answer);
};
