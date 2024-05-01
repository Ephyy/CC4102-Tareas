#include <iostream>
#include "Point.h"

int main()
{
    std::cout << "Hello World" << std::endl;


    Point p = Point(1, 2);
    // Point q = Point(3, 4);
    std::cout << p.get_X() << std::endl;
    std::cout << p.get_Y() << std::endl;

    return 0;
}

