
#include <iostream>
#include <numeric>
#include <map>
#include <algorithm>
#include <vector>
#include <random>
#include <cmath>
#include <chrono> 

#include "Point.h"
#include "Entry.h"
#include "Node.h"
#include "MTreeCP.h"

using namespace std;

std::ostream& operator<<(std::ostream& os, const std::vector<int>& v) {
    os << "[";
    for (const auto& p : v) {
        os << p << " ";
    }
    os << "]";
    return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<vector<int>>& vv) {
    os << "[";
    for (const auto& v : vv) {
        os << v << " ";
    }
    os << "]";
    return os;
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

std::ostream& operator<<(std::ostream& os, const std::map<int, vector<int>>& m) {
    os << "{\n";
    for (const auto& [key, value] : m) {
        os << "    " << key << "=> " << value << "\n";
    }
    os << "}";
    return os;
}

std::ostream& operator<<(std::ostream& os, const std::map<int, Node>& n) {
    os << "{";
    for (const auto& [key, value] : n) {
        os << key << "=> " << value << " ";
    }
    os << "}";
    return os;
}

std::ostream& operator<<(std::ostream& os, const std::map<int, std::shared_ptr<Node>>& n) {
    os << "{\n";
    for (const auto& [key, value] : n) {
        os << "    " << key << " => " << *value << "\n";
    }
    os << "}";
    return os;
}


// Función que dado un punto, devuelve su índice en un vector de puntos
int findPointIndex(const Point& point, const vector<Point>& points) {
    auto it = find(points.begin(), points.end(), point);
    if (it != points.end()) {
        return distance(points.begin(), it);
    }
    return -1; // Si el punto no se encuentra en el vector
}


// Entrega el valor samples[f_i] más cercano al punto p_i en el vector de puntos points
int nearestPoint(int p_i, vector<int>& samples, vector<Point>& points) {
    double minDist = dist(points[p_i], points[samples[0]]);
    int nearest = samples[0];
    for (int f_i = 1; f_i < samples.size(); f_i++) {
        double d = dist(points[p_i], points[samples[f_i]]);
        if (d < minDist) {
            minDist = d;
            nearest = samples[f_i];
        }
    } 
    return nearest;
}

// ## Paso 2) del metodo CP
// Se seleccionan K puntos aleatorios de la lista de puntos.
//
// Returns:
// Entrega el conjunto de samples F (los indices de los puntos seleccionados)
//
// Ej: selectRandomPoints([(1, 2) (2, 3) (3, 4) (5, 6) ... ], B=2) -> [0, 3] 
vector<int> selectRandomPoints(vector<int>& index_points, int B) {
    // Calcular el número de puntos a seleccionar
    int n = index_points.size();
    double t = static_cast<double>(n) / B;
    int k = std::min(B, static_cast<int>(std::ceil(t)));

        
    // Hacer una copia del vector de índices
    std::vector<int> indicesCopia = index_points;

    // Barajar el vector
    std::random_device rd;  // Fuente de entropía
    std::mt19937 gen(rd());  // Generador Mersenne Twister
    std::shuffle(indicesCopia.begin(), indicesCopia.end(), gen);

    // Seleccionar los primeros k elementos
    vector<int> samples(indicesCopia.begin(), indicesCopia.begin() + k);

    return samples;
}


// ## Paso 3 y 4) del metodo CP
// Se asigna cada punto a su sample más cercano y en caso de que un sample tenga menos de b puntos, se redistribuyen los puntos
// 
// Returns:
// Entrega un vector con los conjuntos F_k: Los puntos más cercanos a cada sample
map<int, vector<int>> nearestSample(vector<Point>& points, vector<int>& index_points, vector<int>& samples, double b) {
    map<int, vector<int>> F;


    for (const int& p_i : index_points) {
        int nearestSamp = nearestPoint(p_i, samples, points); // Encuentra el sample más cercano
        F[nearestSamp].push_back(p_i); // Agrega p al conjunto de puntos del sample más cercano
    }


    // Paso 4:
    // Se redistribuyen los puntos si un sample tiene menos de b puntos
    vector<int> samplesCopia = samples;
    for (const int& s : samples) {
        if (F[s].size() < b) {
            // Obtenemos los puntos a redistribuir
            vector<int> toDistribute = F[s];

            // Lo boramos de la lista de samples
            samplesCopia.erase(std::remove(samplesCopia.begin(), samplesCopia.end(), s), samplesCopia.end());
            F.erase(s); // Borramos el sample del map

            // Distribuimos los puntos en el resto de los samples
            for (const int& p_i : toDistribute) {
                int nearestSamp = nearestPoint(p_i, samplesCopia, points);
                // Agregamos el punto al conjunto de puntos del sample más cercano
                F[nearestSamp].push_back(p_i);

            }
        }
    }
 
    samples = samplesCopia;

    return F;
}

// Funcion auxiliar para los pasos 2 al 5
map<int, vector<int>> getFinalSamples(vector<int>& index_point, vector<Point>& points, double B, double b) {

    // Paso 2
    vector<int> samples = selectRandomPoints(index_point, B); 
    // Paso 3 y 4
    map<int, vector<int>> nearestSamples = nearestSample(points, index_point, samples, b); // Paso 3 y 4

    // Paso 5: Si solo hay un punto en el conjunto volver a paso 2
    int count = 0;
    while (nearestSamples.size() == 1) {
        samples = selectRandomPoints(index_point, B); // Paso 2
        nearestSamples = nearestSample(points, index_point, samples, b); // Paso 3 y 4
        count++;
        if (count > 5) {
            break;
        }
    }

    return nearestSamples;
}


// BulkLoading
//
// Algoritmo recursivo que construye un arbol CP a partir de un conjunto de puntos
//
// Returns:
//  El arbol T de un conjunto de puntos
Node BulkLoading(vector<int> index_point, vector<Point> points, double B) {

    // Paso 1)
    if (index_point.size() <= B) {
        Node node = Node(B);
        for (const int& p_i : index_point) {
            Entry entry = Entry(points[p_i], 0, nullptr);
            node.insert(entry);
        }
        return node;
    } 

    // Construyo el arbol T
    Node tree = Node(B);

    // Paso 2 al 5
    map<int, vector<int>> nearestSamples = getFinalSamples(index_point, points, B, tree.get_b());

    // Paso 6: Se realiza recursivamente el algoritmo para cada conjunto de puntos
    for (const auto& [s, F] : nearestSamples) {
        auto a = std::make_shared<Node>(BulkLoading(F, points, B));
        Entry entry = Entry(points[s], 0, a);
        tree.insert(entry);
    }

    return tree;
}


// Funcion auxliar del paso 9 que realiza una busqueda exhaustiva en el arbol
// para encontrar todos los sub arboles de altura h
void splitIntoSubTrees(map<int, std::shared_ptr<Node>>& newSubTrees, int s, std::shared_ptr<Node>& tree, int h, vector<Point>& points) {
    // Caso base:
    // Si la altura del arbol es igual a h, se agrega a subTrees
    if (tree->height() == h) {
        newSubTrees[s] = tree;
    } else {
        // Caso recursivo:
        // Se recorre el arbol en busca de sub arboles de altura h
        if (!tree->is_leaf()) {
            for (const auto& entry : tree->get_entries()) {
                // Obtengo el punto
                Point p = entry.get_p();
                // Obtengo el indice
                int new_sample = findPointIndex(p, points);
                // Obtengo el sub arbol
                std::shared_ptr<Node> subTree = entry.get_a();
                // Llamada recursiva
                splitIntoSubTrees(newSubTrees, new_sample, subTree, h, points);
            }
        }
    }
}

// Funcion auxiliar para el paso 11
// Dado Tsup y un map de sub arboles, se agregan los sub arboles en las hojas de Tsup
void addSubTrees(Node& Tsup, map<int, std::shared_ptr<Node>>& subTrees, vector<Point>& points) {
    if (Tsup.is_leaf()) {
        // Reviso las entradas de Tsup
        for (Entry& entry : Tsup.get_entries()) {
            // Obtengo el punto
            Point p = entry.get_p();
            // Obtengo el indice
            int new_sample = findPointIndex(p, points);
            // Obtengo el sub arbol
            std::shared_ptr<Node> subTree = subTrees[new_sample];
            // Agrego el sub arbol
            entry.set_a(subTree);
        }
    } else {
        // Llamada recursiva
        for (Entry& entry : Tsup.get_entries()) {
            addSubTrees(*entry.get_a(), subTrees, points);
        }
    }
}


// Algoritmo 
std::shared_ptr<Node> cpAlgorithm(vector<Point>& points, double B, double b) {
    cout << "TreeCP: Creando CP Tree" << endl;
    int n = points.size();

    // Paso 1
    vector<int> index_point(n);
    std::iota(index_point.begin(), index_point.end(), 0);

    // Paso 2 al 5
    map<int, vector<int>> nearestSamples = getFinalSamples(index_point, points, B, b);

    // Paso 6, se realiza recursivamente el algoritmo para cada conjunto de puntos
    map<int, std::shared_ptr<Node>> subTrees; // Sub arboles de los samples
    for (const auto& [s, F] : nearestSamples) {
        auto a = std::make_shared<Node>(BulkLoading(F, points, B));
        subTrees[s] = a;
    }

    // Paso 7
    // Si la raiz de un sub arbol es de un tamaño menor a b,
    // se elimina el sample
    // se trabaja con sus subarboles como nuevos arboles
    // se añaden los puntos pertinentes de estos sub arboles como samples

    auto it = subTrees.begin();
    while (it != subTrees.end()) {
        int s = it->first;
        std::shared_ptr<Node> subT = it->second;
        int childs = subT->get_entries().size();

        if (childs < subT->get_b()) {

            // Elimino el sample del vector del map
            it = subTrees.erase(it);


            // Agrego los sub arboles como nuevos arboles
            for (const auto& entry : subT->get_entries()) {
                // Obtenemos el punto
                Point p = entry.get_p();
                // Obtenemos el indice
                int new_sample = findPointIndex(p, points);
                // Obtenemos el sub arbol
                std::shared_ptr<Node> a = entry.get_a();
                // Agregamos el sub arbol
                subTrees[new_sample] = a;
            }

        } else {
            it++;
        }

    }
    
    // Paso 8 : Calcular la altura mínima de los árboles en subTrees
    int h = INT_MAX;
    for (const auto& [s, subT] : subTrees) {
        int height = subT->height();
        if (height < h) {
            h = height;
        }
    }

    // Paso 9:
    // Por cada subTree si su altura es igual a h, se agrega a newSubTrees
    // En caso contrario:
    // 1. Se borra el sample
    // 2. Se hace un busqueda exhaustiva en el arbol para encontrar todos los sub arboles de altura h. Se agregan a newSubTrees
    // 3. Se agregan los puntos pertinentes de estos sub arboles como samples
    map<int, std::shared_ptr<Node>> newSubTrees;
    it = subTrees.begin();
    while (it != subTrees.end()) {
        int s = it->first;
        std::shared_ptr<Node> subT = it->second;
        if (subT->height() == h) {
            newSubTrees[s] = subT;
            it++;
        } else {
            // Elimino el sample
            it = subTrees.erase(it);
            // Busqueda exhaustiva
            splitIntoSubTrees(newSubTrees, s, subT, h, points);
        }
    }

    // Paso 10, se calcula el arbol Tsup a partir de los samples que quedaron
    vector<int> finalSamples;
    for (const auto& [s, subT] : newSubTrees) {
        finalSamples.push_back(s);
    }

    Node T_sup = BulkLoading(finalSamples, points, B);

    // Paso 11, agregar en las hojas de Tsup los sub arboles de newSubTrees
    // Recoremos el arboll Tsup
    addSubTrees(T_sup, newSubTrees, points);

    // Paso 12, actualizar el radio de cobertura de cada entrada en el arbol
    std::shared_ptr<Node> Tree = std::make_shared<Node>(T_sup);
    set_covering_radius(Tree);
    cout << "TreeCP: CP Tree creado" << endl;
    return Tree;
}


MTreeByCP::MTreeByCP(double B) : root(nullptr), b(B*0.5), B(B) {}

void MTreeByCP::set_node(vector<Point>& points) {
    this->root = cpAlgorithm(points, this->B, this->b);
}



// Main 
// int main() {
//     // Capturar el tiempo de inicio
//     auto inicio = std::chrono::high_resolution_clock::now();

//     // Crear un vector de puntos
//     int n = 64;
//     vector<Point> points = generateRandomPoints(n, 1);
    
//     // Crear un arbol CP
//     MTreeByCP Tree(4);
//     Tree.set_node(points);
    
//     // Node Tree = BulkLoading({0, 1, 2, 3, 4, 5, 6, 7}, points, B);
//     // cout << "Arbol CP: " << Tree << endl;
//     // updateCoveringRadius(Tree);
//     // cout << "Arbol CP: " << Tree << endl;


//     // Capturar el tiempo de finalización
//     auto fin = std::chrono::high_resolution_clock::now();
//     // Calcular la diferencia de tiempo
//     std::chrono::duration<double> duracion = fin - inicio;
//     std::cout << "Tiempo de ejecución: " << duracion.count() << " segundos" << std::endl;
    
//     return 0;
// }

