#ifndef NODE_H
#define NODE_H

#include <set>
#include <memory>
#include "Entry.h" // Include the missing header file for the 'Entry' class


class Entry; // Forward declaration of the 'Entry' class

class Node {
private:
    std::vector<Entry> entries;
    double b;
    double B;

public:
    // Constructor
    Node(double B);

    // Getters
    double get_b();
    double get_B();
    std::vector<Entry> get_entries();

    void insert(Entry e);
};

#endif