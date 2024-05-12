#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>


class MTreeByCP {
    public:
        shared_ptr<Node> root;
        double b;
        double B;

        MTreeByCP(double B);

        void set_node(vector<Point>& points);
};

