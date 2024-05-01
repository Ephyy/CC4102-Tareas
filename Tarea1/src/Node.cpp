#include <iostream>
#include "Node.h"


// Constructor
Node::Node(double b, double B) : entries(), b(b), B(B) {}

// Getters
double Node::get_b() {
    return b;
}

double Node::get_B() {
    return B;
}