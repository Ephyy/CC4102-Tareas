#include <iostream>
#include "Point.h"
#include "Node.h"
#include "Entry.h"

int main()
{
    std::cout << "TESTING" << std::endl;


    Point p = Point(1, 2);
    Node n1 = Node(2);
    Entry e1 = Entry(p, 3, n1);

    std::cout << e1.get_a().get_B() << std::endl;

    return 0;
}

