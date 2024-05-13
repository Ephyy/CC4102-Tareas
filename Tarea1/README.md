# Tarea 1: M-Tree

Este proyecto implementa un M-Tree, una estructura de datos especializada para la indexación de datos en un espacio métrico.

## Requisitos
- Compilador que soporte C++11 o superior
- Extensión de C++ para Visual Studio Code

## Configuración del proyecto
El proyecto se configura y compila utilizando la extensión de C++ para Visual Studio Code.

## Tutorial de compilación en Windows y Linux
Puede encontrar un tutorial detallado sobre cómo compilar y ejecutar un proyecto en Windows y Linux en el siguiente enlace: 
- [Windows](https://code.visualstudio.com/docs/cpp/config-mingwn)
- [Linux](https://code.visualstudio.com/docs/cpp/config-linuxn)


### Configuracion tasks.json

La extension de C++, genera varios archivos `.json` para compilar un proyecto. En particular para el archivo `tasks.json` se recomienda incluir los siguientes argumentos:

```json
    "tasks": [
        {
            ...
            "args": [
                "-fdiagnostics-color=always",
                "-g",
                "-std=c++20",
                "-I",
                "${workspaceFolder}/include",
                "${workspaceFolder}/src/*.cpp",
                "-o",
                "${fileDirname}\\${fileBasenameNoExtension}.exe"
            ],
            ...
        }
    ]
```
De esta forma se incluye la flag `-I` para incluir los archivos `.h`.

## Ejecucion
El codigo se ejecuta directamente desde `main.cpp` utilizando la extension de C++.

## Estructura del proyecto
El proyecto tiene la siguiente estructura de directorios:

- `include/`: Contiene los archivos de cabecera.
- `src/`: Contiene los archivos de código fuente.
- `tests/`: Contiene los archivos de pruebas unitarias.
- `experiments/`: Contiene archivos de experimentos.

## Uso
### Creacion de arboles Mtree
Para crear un MTree se definen dos clases `MTreeByCP` y `MTreeBySS`, las cuales permiten crear un MTree
utilizando el metodo CP y SS respectivamente. Usando de constructor la capacidad maxima `B` de los nodos.


Ejemplo de creacion
```c++
double B = 512 // Capacidad maxima de los nodos
MTreeByCP mtree_cp = MTreeByCP(B); // Crear un MTree que usara el metodo CP de construccion.
MTreeBySS mtree_ss = MTreeBySS(B); // Crear un MTree que usara el metodo ss de construccion
```

### Setear Nodos
Para construir los nodos del arbol, se llama el metodo `.set_node(vector<Point> p)`, el cual recibe un vector de `Point`. Estructura ya definida en el codigo.


```c++

MTreeByCP mtree_cp = MTreeByCP(B); // Crear un MTree que usara el metodo CP de construccion.
vector<Point> points = {Point{1,2}, Point{3,1}, ...} // Vector de puntos
mtree_cp.set_node(points); // Construimos los nodos del arbol utilizando el metodo correspondiente.

```


### Experimentos
Para generar experimentos, se tiene una funcion ` experiment(int method, int n_exp)` en `main.cpp`. La cual genera automaticamente 100 consultas a un arbol construido usando el metodo `method` que se construye con una cantidad del orden de `n_exp` puntos. Lo cual genera un archivo `.tsv` con informacion sobre las consultas realizadas.


```c++
// Ej: Crear experimentos desde 10 hasta 14, para un MTree construido con CP.
for (int i = 10; i <= 14; i++) {
    experiment(CP_METHOD, i);
}
```


## GoogleTest y CMake (Experimental)
El codigo cuenta con un archivo `CMakeLists.txt` para generar un build del proyecto. Actualmente solo es un archivo experimental para compilar una serie de UnitTest, para su posible uso en la siguiente Tarea.



