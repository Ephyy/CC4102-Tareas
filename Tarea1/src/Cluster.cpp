#include <iostream>

using namespace std;

class Cluster {
    public:
        int x;
        int y;
        int z;
        int id;
        Cluster(int x, int y, int z, int id) {
            this->x = x;
            this->y = y;
            this->z = z;
            this->id = id;
        }
        void print() {
            cout << "Cluster " << id << ": (" << x << ", " << y << ", " << z << ")" << endl;
        }
};