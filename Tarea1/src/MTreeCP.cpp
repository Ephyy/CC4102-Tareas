
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

// Paso 3) del algoritmo CP
// Se asigna cada punto a su sample más cercano
vector<vector<Point>> nearestSample(vector<Point>& points, vector<Point>& samples) {
    vector<vector<Point>> nearestSamples(samples.size()); // Crea un vector de vectores de puntos, uno para cada muestra
    
    for (int i = 0; i < points.size(); i++) {
        Point p = points[i];
        double minDist = dist(p, samples[0]);
        int nearestSamp = 0;
        for (int j = 1; j < samples.size(); j++) {
            double d = dist(p, samples[j]);
            if (d < minDist) {
                minDist = d;
                nearestSamp = j;
            }
        }
        nearestSamples[nearestSamp].push_back(p); // Add the point to the vector of nearest points for the corresponding sample
    }
    return nearestSamples;
}


std::ostream& operator<<(std::ostream& os, const std::vector<Point>& v) {
    os << "[";
    for (const auto& p : v) {
        os << p << " ";
    }
    os << "]";
    return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<std::vector<Point>>& vv) {
    os << "[";
    for (const auto& v : vv) {
        os << v << " ";
    }
    os << "]";
    return os;
}

