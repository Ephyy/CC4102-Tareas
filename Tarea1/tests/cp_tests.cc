#include <iostream>

#include <gtest/gtest.h>

#include "Point.h"
#include "Entry.h"
#include "Node.h"
// #include "MTreeCP.h"


using namespace std;

TEST(NodeTest, TestConstructor) {
    Node n(10);
    EXPECT_EQ(n.get_B(), 10);
    EXPECT_EQ(n.get_b(), 5);
}


TEST(NodeTest, Height_test) {
    int B = 10;
    Node n = Node(B);
    EXPECT_EQ(n.height(), 1);

    Entry e = Entry(Point({1, 1}), 0, nullptr);
    n.insert(e);
    EXPECT_EQ(n.height(), 1);


    shared_ptr<Node> n2 = make_shared<Node>(B);
    Entry e_leaf = Entry(Point({3, 3}), 0, nullptr);
    n2->insert(e_leaf);

    Entry e2 = Entry(Point({2, 2}), 0, n2);
    n.insert(e2);

    EXPECT_EQ(n.height(), 2);

    shared_ptr<Node> n3 = make_shared<Node>(B);
    Entry e3 = Entry(Point({4, 4}), 0, n3);
    n2->insert(e3);


    EXPECT_EQ(n.height(), 3);
}

TEST(NodeTest, Validate_test) {
    int B = 10;
    Node n = Node(B);
    vector<Point> points = {Point({1, 1}), Point({2, 2}), Point({3, 3})};

    Entry e = Entry(Point({1, 1}), 0, nullptr);
    n.insert(e);

    vector<Point> points2 = points;
    EXPECT_EQ(n.validate(points2), false);

    shared_ptr<Node> n2 = make_shared<Node>(B);
    Entry e_leaf = Entry(Point({3, 3}), 0, nullptr);
    n2->insert(e_leaf);

    Entry e2 = Entry(Point({2, 2}), 0, n2);
    n.insert(e2);

    vector<Point> points3 = points;
    EXPECT_EQ(n.validate(points3), true);
}

