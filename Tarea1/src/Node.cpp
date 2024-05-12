#include <iostream>
#include <algorithm>
#include <vector>

#include "Point.h"
#include "Entry.h"
#include "Node.h"

using namespace std;


// Constructor
Node::Node(double B) : entries(), b(B*0.5), B(B) {}

bool Node::is_leaf() const
{   
    for (int i = 0; i < entries.size(); i++)
    {
        if (entries[i].get_a() != nullptr)
        {
            return false;
        }
    }
    return true;
}


// Height
// Calcula la altura del del arbol
int Node::height() const
{   
    if (is_leaf())
    {
        return 1;
    }
    else
    {
        int max = 0;
        for (const auto& entry : entries)
        {
            if (entry.get_a() != nullptr)
            {
                int h = entry.get_a()->height();
                if (h > max)
                {
                    max = h;
                }
            }
        }

        return 1 + max;
    }
}

// Checks if the node contains all the points
bool Node::validate(vector<Point>& points) const
{
    return false;
}

// Getters
std::vector<Entry> Node::get_entries() const {
    return entries;
}

double Node::get_b() const {
    return b;
}

double Node::get_B() {
    return B;
}

std::vector<Entry> Node::get_entries() {
    return entries;
}

void Node::insert(Entry e) {
    entries.push_back(e);
}

// Print
std::ostream& operator<<(std::ostream& os, const Node& n) {
    os << "Node(" << n.get_entries() << ")";
    return os;
}


// Se setean los radios cobertores resultantes para cada 
// entrada en este árbol.
void set_covering_radius(shared_ptr<Node> node) {
    if (node->is_leaf()) {
        // Cada hoja tiene un radio cobertor de 0
        for (Entry &entry : node->get_entries()) {
            entry.set_cr(0);
        }
    } else {
        for (Entry &entry : node->get_entries()) {
            // Seteo el radio del nodo hijo
            set_covering_radius(entry.get_a());
            // Calculo el radio de la entrada actual
            double max_radius = 0;
            for (Entry child_entry : entry.get_a()->get_entries()) {
                max_radius = max(max_radius, dist(entry.get_p(), child_entry.get_p()) + child_entry.get_cr());
            }
            entry.set_cr(max_radius);
        }
    }
    return;
}
