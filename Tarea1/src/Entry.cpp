#include "Entry.h"
#include "Node.h"

// Constructor
Entry::Entry(Point p, double cr, std::shared_ptr<Node> a) : p(p), cr(cr), a(a) {}

Point Entry::get_p() const {
    return p;
}

double Entry::get_cr() const {
    return cr;
}

std::shared_ptr<Node> Entry::get_a() const {
    return a;
}

// Print de una Entry
std::ostream& operator<<(std::ostream& os, const Entry& e) {
    if (e.get_a()) {
        os << "E(" << e.get_p() << ", " << e.get_cr() << ", " << *e.get_a() << ")";
    } else {
        os << "E(" << e.get_p() << ", " << e.get_cr() << ", NULL)";
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<Entry>& entries) {
    os << "[";
    for (const auto& e : entries) {
        os << e << ", ";
    }
    os << "]";
    return os;
}