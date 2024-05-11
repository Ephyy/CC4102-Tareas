#ifndef ENTRY_H
#define ENTRY_H 

#include <iostream>
#include <memory>
#include "Point.h"

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
    Point get_p() const;
    double get_cr() const;
    std::shared_ptr<Node> get_a() const;
};

// Print
std::ostream& operator<<(std::ostream& os, const Entry& e);
std::ostream& operator<<(std::ostream& os, const std::vector<Entry>& entries);

#endif
