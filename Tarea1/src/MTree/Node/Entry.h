#include <iostream>
#include "Point.h"

using namespace std;

class Node;

struct Entry {
    Point p;
    double cr;
    Node &a;
};
