#ifndef NODE_H
#define NODE_H

#include <set>
#include <memory>
#include <vector>


class Entry;

class Node {
private:
    std::vector<Entry> entries;
    double b;
    double B;

public:
    // Constructor
    Node(double B);

    // Leaf
    bool is_leaf() const;
    // Height
    int height() const;
    // Checks if the node contains all the points
    bool validate(vector<Point>& points) const;

    // Getters
    std::vector<Entry> get_entries() const;
    double get_b() const;
    double get_B();
    std::vector<Entry>& get_entries();

    // Setters
    void insert(Entry e);
};


// void update_cr(Node n);

// Print
std::ostream& operator<<(std::ostream& os, const Node& n);

// Update cr
void set_covering_radius(shared_ptr<Node> node);

#endif