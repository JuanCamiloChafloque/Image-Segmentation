#include "Graph.h"
#include "Imagen.h"
#include <vector>
#include <string>
#include <iostream>
#include <queue>
#include <fstream>
#include <string>
#include <sstream>
#include <string.h>
#include <algorithm>

template <class T, class C>
Grafo<T, C>::Grafo(Imagen &imagen)
{
    this->imagen = imagen;
    int idVertice = 0;
    for (int i = 0; i < this->imagen.getH(); i++)
    {
        for (int j = 0; j < this->imagen.getW(); j++)
        {
            insertarVertice(idVertice);
            idVertice++;
        }
    }
}

template <class T, class C>
void Grafo<T, C>::crearGrafo(std::vector<Semilla> &semillas, std::string &archivo_ppm)
{
    unsigned short W = this->imagen.getW();
    unsigned short H = this->imagen.getH();
    // Itero la imagen e inserto aristas (arriba, abajo, izquierda, derecha) y su respectivo costo
    for (int i = 0; i < this->numVertices; i++)
    {
        unsigned short row = i / W;
        unsigned short col = i - (row * W);

        unsigned short pixel = this->imagen.getPixel(row, col);
        // Vecino de arriba
        if (row - 1 >= 0)
        {
            unsigned short x = row - 1;
            unsigned short upPixel = this->imagen.getPixel(x, col);
            int upId = (row - 1) * W + col;
            double peso = pixel - upPixel < 0 ? upPixel - pixel : pixel - upPixel;
            insertarArista(i, upId, peso);
        }
        // Vecino de abajo
        if (row + 1 < H)
        {
            unsigned short x = row + 1;
            unsigned short downPixel = this->imagen.getPixel(x, col);
            int downId = (row + 1) * W + col;
            double peso = pixel - downPixel < 0 ? downPixel - pixel : pixel - downPixel;
            insertarArista(i, downId, peso);
        }
        // Vecino de la izquierda
        if (col - 1 >= 0)
        {
            unsigned short y = col - 1;
            unsigned short leftPixel = this->imagen.getPixel(row, y);
            int leftId = row * W + (col - 1);
            double peso = pixel - leftPixel < 0 ? leftPixel - pixel : pixel - leftPixel;
            insertarArista(i, leftId, peso);
        }
        // Vecino de la derecha
        if (col + 1 < H)
        {
            unsigned short y = col + 1;
            unsigned short rightPixel = this->imagen.getPixel(row, y);
            int rightId = row * W + (col + 1);
            double peso = pixel - rightPixel < 0 ? rightPixel - pixel : pixel - rightPixel;
            insertarArista(i, rightId, peso);
        }
    }

    std::cout << "\nGrafo inicializado.";

    // Por cada semilla en el vector, llamo al método de Dijsktra para realizar el algoritmo
    for (int i = 0; i < semillas.size(); i++)
    {
        std::cout << "\nIniciando algoritmo de Disjkstra con las semillas #: " << i + 1;
        unsigned short x = semillas[i].getX();
        unsigned short y = semillas[i].getY();
        unsigned short l = semillas[i].getL();
        std::vector<C> d = realizarDijkstra(x, y);
        // Volver a recorrer cada pixel de la imagen y guardar el mejor (menor) valor dependiendo del vector d
        actualizarImagen(d, l);
    }

    // Generar archivo PGM con la imagen actualizada
    generarArchivo(archivo_ppm);
}

template <class T, class C>
void Grafo<T, C>::generarArchivo(std::string &archivo_ppm)
{
    std::ofstream salida;
    int X = this->imagen.getH();
    int Y = this->imagen.getW();
    bool primero = false;
    int mayorM = 0;
    for (unsigned short i = 0; i < X; i++)
    {
        for (unsigned short j = 0; j < Y; j++)
        {
            unsigned short actual = this->imagen.getPixel(i, j);
            if (actual > mayorM)
            {
                mayorM = actual;
            }
        }
    }
    salida.open(archivo_ppm, std::ios::out);
    salida << "P2" << std::endl;
    salida << Y << " " << X << std::endl;
    salida << mayorM << std::endl;
    for (unsigned short a = 0; a < X; a++)
    {
        if (primero)
            salida << std::endl;
        for (unsigned short z = 0; z < Y; z++)
        {
            unsigned short actual = this->imagen.getPixel(a, z);
            salida << actual << " ";
            primero = true;
        }
    }
    salida.close();
}

template <class T, class C>
void Grafo<T, C>::actualizarImagen(std::vector<C> &dist, unsigned short &l)
{
    unsigned short W = this->imagen.getW();
    for (int i = 0; i < this->numVertices; i++)
    {
        unsigned short row = i / W;
        unsigned short col = i - (row * W);

        // Obtengo el valor del pixel actual del vector de distancias producido por el Dijkstra
        double val = dist[i];

        // Verifico si el valor actual de distancia de ese pixel en la imagen es menor o mayor a 'val'. Si 'val' es menor, se actualiza
        if (val < this->imagen.getDistancia(i))
        {
            this->imagen.setDistancia(i, val);
            this->imagen.setPixel(row, col, l);
        }
    }
}

template <class T, class C>
std::vector<C> Grafo<T, C>::realizarDijkstra(unsigned short &x, unsigned short &y)
{
    std::vector<C> dist(this->numVertices, 100000);
    std::vector<bool> isPath(this->numVertices, false);

    int ori = x * this->imagen.getW() + y;
    dist[ori] = 0;

    for (int count = 0; count < this->numVertices - 1; count++)
    {
        int u = minDistance(dist, isPath);
        isPath[u] = true;
        for (int v = 0; v < this->numVertices; v++)
        {
            C costo = buscarCostoArista(u, v);
            if (!isPath[v] && buscarArista(u, v) != -1 && dist[u] != 100000 && dist[u] + costo < dist[v])
            {
                dist[v] = dist[u] + costo;
            }
        }
    }

    return dist;
}

template <class T, class C>
int Grafo<T, C>::minDistance(std::vector<C> &dist, std::vector<bool> &isPath)
{
    int min = 100000;
    int minIndex;

    for (int v = 0; v < this->numVertices; v++)
    {
        if (isPath[v] == false && dist[v] <= min)
        {
            min = dist[v];
            minIndex = v;
        }
    }
    return minIndex;
}

template <class T, class C>
void Grafo<T, C>::insertarVertice(T vertice)
{
    if (this->numVertices == 0 || buscarVertice(vertice) == -1)
    {
        this->vertices.push_back(vertice);
        std::vector<std::pair<T, C>> aux;
        this->aristas.push_back(aux);
        this->numVertices = this->vertices.size();
    }
}

template <class T, class C>
void Grafo<T, C>::insertarArista(T origen, T destino, C peso)
{
    int ori = this->vertices[origen];
    int des = this->vertices[destino];
    if (ori != -1 && des != -1)
    {
        std::vector<std::pair<T, C>> *aux = &aristas[ori];
        std::pair<T, C> nAri(des, peso);
        aux->push_back(nAri);
    }
}

template <class T, class C>
int Grafo<T, C>::buscarVertice(T vertice)
{
    int indice = -1;
    for (int i = 0; i < numVertices && indice == -1; i++)
    {
        if (vertice == vertices[i])
            indice = i;
    }
    return indice;
}

template <class T, class C>
int Grafo<T, C>::buscarArista(T origen, T destino)
{
    int inOrigen = this->vertices[origen];
    int encontrado = -1;
    std::vector<std::pair<T, C>> aux = aristas[inOrigen];
    for (int i = 0; i < aux.size() && encontrado == -1; i++)
    {
        std::pair<T, C> temp = aux[i];
        if (temp.first == destino)
            encontrado = i;
    }
    return encontrado;
}

template <class T, class C>
C Grafo<T, C>::buscarCostoArista(T origen, T destino)
{
    int inOrigen = this->vertices[origen];
    C costo = -1;
    std::vector<std::pair<T, C>> aux = aristas[inOrigen];
    for (int i = 0; i < aux.size() && costo == -1; i++)
    {
        std::pair<T, C> temp = aux[i];
        if (temp.first == destino)
            costo = temp.second;
    }
    return costo;
}
