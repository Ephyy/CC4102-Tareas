#include <iostream>
#include <vector>
#include <algorithm>
#include "Node/Entry.h"

using namespace std;

class Node {
    public:
        vector<Entry> entries;
        // Node *parent;
        bool is_leaf;
        double B; // max_entries
        double b; // min_entries

        Node(double B) {
            this->B = B;
            this->b = 0.5*B;
            // this->parent = nullptr;
            // this->is_leaf = true;
        }

        // copilot: 
        
        // bool is_overflow() {
        //     return entries.size() > B;
        // }

        // bool is_underflow() {
        //     return entries.size() < b;
        // }

        // void add_entry(Entry entry) {
        //     entries.push_back(entry);
        // }

        // void remove_entry(Entry entry) {
        //     entries.erase(remove(entries.begin(), entries.end(), entry), entries.end());
        // }

        // void sort_entries() {
        //     sort(entries.begin(), entries.end(), [](Entry a, Entry b) {
        //         return a.cr < b.cr;
        //     });
        // }

        // void print_entries() {
        //     for (auto entry : entries) {
        //         cout << entry.cr << " ";
        //     }
        //     cout << endl;
        // }
};