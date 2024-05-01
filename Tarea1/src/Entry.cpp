#include <Entry.h>

// Constructor
Entry::Entry(Point p, double cr, Node& a) : p(p), cr(cr), a(a) {}

Point Entry::get_p() {
    return p;
}

double Entry::get_cr() {
    return cr;
}

Node& Entry::get_a() {
    return a;
}
