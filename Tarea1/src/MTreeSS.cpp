#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cassert>
#include <queue>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <random>
#include <functional>
#include <numeric>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "Entry.h"

using namespace std;

class MTreeBySS {
    public:
        Node &root;
        double B;
        double b;

        MTreeBySS(double B, double b, Node &root) : root(root) {
            this->B = B;
            this->b = b;
            // root = new MTreeNode();
        }

        // Node add_point(Node cur, vector<int> key) {
        //     if (cur->children.find(key) == cur->children.end()) {
        //         cur->children[key] = new MTreeNode();
        //     }
        //     return cur->children[key];
        // }

        // void cluster(Node cur, vector<int> key) {
        //     if (cur->children.size() == 0) {
        //         cur->children[key] = new MTreeNode();
        //         return;
        //     }
        //     double min_dist = 1e9;
        //     Node best_child = nullptr;
        //     for (auto& child : cur->children) {
        //         double dist = 0;
        //         for (int i = 0; i < key.size(); i++) {
        //             dist += (child.first[i] - key[i]) * (child.first[i] - key[i]);
        //         }
        //         if (dist < min_dist) {
        //             min_dist = dist;
        //             best_child = child.second;
        //         }
        //     }
        //     if (min_dist < b) {
        //         cluster(best_child, key);
        //     } else {
        //         cur->children[key] = new MTreeNode();
        //     }
        // }


        void cluster(Node cur, vector<int> key) {
            if (cur->children.size() == 0) {
                cur->children[key] = new MTreeNode();
                return;
            }
            double min_dist = 1e9;
            Node best_child = nullptr;
            for (auto& child : cur->children) {
                double dist = 0;
                for (int i = 0; i < key.size(); i++) {
                    dist += (child.first[i] - key[i]) * (child.first[i] - key[i]);
                }
                if (dist < min_dist) {
                    min_dist = dist;
                    best_child = child.second;
                }
            }
            if (min_dist < b) {
                cluster(best_child, key);
            } else {
                cur->children[key] = new MTreeNode();
            }
        }

        


};