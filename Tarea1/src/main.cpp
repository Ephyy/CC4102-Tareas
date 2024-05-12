#include <iostream>
#include <memory>
#include <chrono>
#include <fstream> // csv
#include <chrono>  // Tiempo
#include <string>

#include "Point.h"
#include "Node.h"
#include "Entry.h"
#include "MTreeCP.h"
#include "MTreeSS.h"
#include "RangeQuery.h"

using namespace std;

const int MAX_BYTES = 4096;   
const double RANGE_QUERY = 0.02; // retorna aproximadamente un 0.12% de los puntos del conjunto
const int QUERIES_SEED = 3;
const int CP_METHOD = 1;
const int SS_METHOD = 2;

// Genera el arbol
// genera archivo de arbol
// 100 puntos
// archivo de consultas
void experiment(int method, int n_exp) {
    int entry_bytes = sizeof(Entry);	
    int max_entries = MAX_BYTES / entry_bytes;

    MTreeByCP mtree_cp = MTreeByCP(max_entries);
    MTreeBySS mtree_ss = MTreeBySS(max_entries);
    shared_ptr<Node> root;
    string method_str = (method == CP_METHOD) ? "CP" : "SS";

    // Archivo log de creación
    std::ofstream creation_file;
    string filename = "..\\experiments\\creation\\creation_" + method_str + "_" + to_string(n_exp) + ".txt";
    creation_file.open(filename);

    // Crear los puntos
    vector<Point> points = generateRandomPoints(pow(2, n_exp), n_exp);
    // Capturar el tiempo de inicio
    auto inicio_creation = std::chrono::high_resolution_clock::now();
    // Imprimir la hora y minuto de inicio
    auto inicio_creation_time = std::chrono::system_clock::to_time_t(inicio_creation);
    std::cout << "Inicio de creación: " << std::ctime(&inicio_creation_time);
    if (method == CP_METHOD) {
        cout << "M-Tree by CP algorithm" << endl;
        // Crear el arbol
        mtree_cp.set_node(points);
        root = mtree_cp.root;
    } 
    
    if (method == SS_METHOD) {
        cout << "M-Tree by SS algorithm" << endl;
        // Crear el arbol
        vector<shared_ptr<Point>> shared_points;
        for (Point p : points) {
            shared_points.push_back(make_shared<Point>(p));
        }
        points.clear(); 
        mtree_ss.set_node(shared_points);
        root = mtree_ss.root;
    }
    // Capturar el tiempo de finalización
    auto fin_creation = std::chrono::high_resolution_clock::now();
    // Calcular la diferencia de tiempo
    std::chrono::duration<double> duracion_creation = fin_creation - inicio_creation; // En segundos
    creation_file << "time: " + to_string(duracion_creation.count()) + "\n";
    creation_file.close();

    
    // Archivo de consultas
    std::ofstream file;
    filename = "..\\experiments\\experimento_" + method_str + "_" + to_string(n_exp) + ".tsv";
    file.open(filename);
    file << "id_query\tq\tios\tsize\tanswer\ttime\n";

    
    // Crear las 100 consultas
    vector<Point> points_query = generateRandomPoints(100, QUERIES_SEED);
    int i = 0;
    for (Point p : points_query) {
        // Capturar el tiempo de inicio
        auto inicio = std::chrono::high_resolution_clock::now();

        RangeQuery query = RangeQuery(p, RANGE_QUERY);
        vector<Point> answer;
        int ios = query.lookup(root, answer);

        string answer_str = "";
        for (Point p : answer) {
            answer_str += "(" + to_string(p.get_x()) + ", " + to_string(p.get_y()) + ")" + "; ";
        }

        // Capturar el tiempo de finalización
        auto fin = std::chrono::high_resolution_clock::now();
        // Calcular la diferencia de tiempo
        std::chrono::duration<double> duracion = fin - inicio; // En segundos
        
        // Escribir datos en el archivo
        string id_query = to_string(i);
        string point = "(" + to_string(p.get_x()) + ", " + to_string(p.get_y()) + ")";
        string ios_str = to_string(ios);
        string size = to_string(answer.size());
        string time = to_string(duracion.count());
        file << id_query + "\t" + point + "\t" + ios_str + "\t" + size + "\t" + answer_str + "\t" + time + "\n";
        i++;
    }   

    // Cerrar el archivo
    file.close();
    return;
}

int main() {
    for (int i = 11; i <= 14; i++) {
        experiment(CP_METHOD, i);
    }
    return 0;
}

