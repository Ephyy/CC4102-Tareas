
#include <iostream>
#include <map>
#include <vector>
#include <random>
#include <cmath>
#include "Point.h"

using namespace std;

// Entrega el indice del punto más cercano a p en el vector de puntos points
int nearestPoint(Point p, vector<Point>& points) {
    double minDist = dist(p, points[0]);
    int nearest = 0;
    for (int i = 1; i < points.size(); i++) {
        double d = dist(p, points[i]);
        if (d < minDist) {
            minDist = d;
            nearest = i;
        }
    }
    return nearest;
}

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
        int nearestSamp = nearestPoint(p, samples); // Encuentra el índice del sample más cercano a p
        nearestSamples[nearestSamp].push_back(p); // Agrega p al conjunto de puntos del sample más cercano
    }
    return nearestSamples;
}

// Paso 4) del algoritmo CP.
// Etapa de redistribución
void cpRedistribution(vector<Point>& samples, vector<vector<Point>>& F, double b) {
    vector<int> toRemove;
    // Por cada conjunto (F_j) en F
    for (int j = 0; j < F.size(); j++) {
        if (F[j].size() < b) {
            // Quitamos el sample p_j de la lista de samples
            Point p_j = samples[j];
            samples.erase(samples.begin() + j);
            toRemove.push_back(j); // Guardamos el índice del conjunto que se eliminará

            // Por cada punto en F[j], lo asignamos al sample más cercano de F y lo agremos a su conjunto de puntos
            for (int i = 0; i < F[j].size(); i++) {
                Point p = F[j][i];
                int nearestSamp = nearestPoint(p, samples);
                F[nearestSamp].push_back(p);
            }
        }
    }

    // Si no hay conjuntos que eliminar, retornamos
    if (toRemove.size() == 0) {
        return;
    }

    // Eliminamos los conjuntos asociado a p_j que fueron eliminados
    for (int i = toRemove.size() - 1; i >= 0; i--) {
        F.erase(F.begin() + toRemove[i]);
    }
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

