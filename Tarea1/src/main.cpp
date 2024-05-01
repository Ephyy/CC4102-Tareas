#include <iostream>
#include <memory>
#include "Point.h"
#include "Node.h"
#include "Entry.h"

int main()
{
    std::cout << "TESTING" << std::endl;


    Point p = Point(1, 2);
    auto n1 = std::make_shared<Node>(2);
    Entry e1 = Entry(p, 3, n1);

    std::cout << e1.get_a()->get_B() << std::endl;

    return 0;
}

