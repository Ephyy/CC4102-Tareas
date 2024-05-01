#ifndef NODE_H
#define NODE_H

#include <set>
#include "Entry.h" // Include the missing header file for the 'Entry' class

class Node {
private:
    std::set<Entry> entries;
    double b;
    double B;

public:
    // Constructor
    Node(double b, double B);

    // Getters
    double get_b();
    double get_B();

};

#endif