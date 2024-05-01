#ifndef ENTRY_H
#define ENTRY_H 

#include <iostream>
#include <memory>
#include "Point.h"
#include "Node.h"

class Node;

class Entry {
private:
    Point p;
    double cr;
    std::shared_ptr<Node> a;
public:
    // Constructor
    Entry(Point p, double cr, std::shared_ptr<Node> a);

    // Getters
    Point get_p();
    double get_cr();
    std::shared_ptr<Node> get_a();
};

#endif
