#include <iostream>
#include "Point.h"
#include "Node.h"

int main()
{
    std::cout << "TESTING" << std::endl;


    Point p = Point(1, 2);
    Node n1 = Node(5, 300);

    std::cout << p.get_Y() << std::endl;

    return 0;
}

