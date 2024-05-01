#include <iostream>
#include "Node.h"


// Constructor
Node::Node(double B) : entries(), b(B*0.5), B(B) {}

// Getters
double Node::get_b() {
    return b;
}

double Node::get_B() {
    return B;
}