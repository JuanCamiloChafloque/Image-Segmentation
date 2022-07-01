#ifndef _GRAFOH_
#define _GRAFOH_

#include "Imagen.h"
#include "Semilla.h"
#include <vector>
#include <string>

template <class T, class C>
class Grafo
{
private:
    std::vector<std::vector<std::pair<T, C>>> aristas;
    std::vector<T> vertices;
    Imagen imagen;
    int numVertices = 0;

public:
    Grafo(Imagen &imagen);
    void crearGrafo(std::vector<Semilla> &semillas, std::string &archivo_ppm);
    std::vector<C> realizarDijkstra(unsigned short &x, unsigned short &y);
    int minDistance(std::vector<C> &dist, std::vector<bool> &isPath);
    void actualizarImagen(std::vector<C> &dist, unsigned short &l);
    void generarArchivo(std::string &archivo_ppm);
    void insertarVertice(T Vertice);
    void insertarArista(T origen, T destino, C peso);
    int buscarVertice(T vertice);
    int buscarArista(T origen, T destino);
    C buscarCostoArista(T origen, T destino);
};

#include "graph.cpp"

#endif