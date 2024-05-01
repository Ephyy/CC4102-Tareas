
#include <iostream>
#include <map>
#include <vector>
#include <random>
#include <cmath>
#include "Point.h"

using namespace std;

// Paso 2) del algoritmo CP
// Se seleccionan K puntos aleatorios de la lista de puntos
vector<Point> selectRandomPoints(vector<Point>& points, int B) {
    vector<Point> selectedPoints;
    int n = points.size();
    int k = std::min(B, n / B);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, n - 1);

    for (int i = 0; i < k; i++) {
        int randomIndex = dist(gen);
        selectedPoints.push_back(points[randomIndex]);
        points.erase(points.begin() + randomIndex);
        n--;
    }

    return selectedPoints;
}

// Main
int main() {
    vector<Point> points = generateRandomPoints(10, 100);
    vector<Point> selectedPoints = selectRandomPoints(points, 2);
    for (int i = 0; i < selectedPoints.size(); i++)
        std::cout << selectedPoints[i] << std::endl;

    std::cout << points.size() << std::endl;
    std::cout << selectedPoints.size() << std::endl;
    return 0;
}