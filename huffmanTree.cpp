#include <iostream>
#include <queue>
#include <list>
#include <vector>
#include <sstream>
#include <map>
#include <fstream>
#include <string.h>
#include <string>
#include <cstring>
#include "Nodo.h"
#include "Imagen.h"
#include "HuffmanTree.h"

struct mayorFrecuencia
{
    bool operator()(Nodo *n1, Nodo *n2)
    {
        if (n1->getFrecuency() == n2->getFrecuency())
        {
            return n1->getSimbolo() > n2->getSimbolo();
        }
        else
        {
            return n1->getFrecuency() > n2->getFrecuency();
        }
    }
};

HuffmanTree::HuffmanTree()
{
    this->raiz = NULL;
}

void HuffmanTree::construirArbol(Imagen &imagen_ppm, std::string archivo_huffman)
{
    std::priority_queue<Nodo *, std::vector<Nodo *>, mayorFrecuencia> mHeap;
    int W = imagen_ppm.getW();
    int H = imagen_ppm.getH();
    unsigned long frecuencias[256];

    for (unsigned long i = 0; i < 256; i++)
    {
        frecuencias[i] = 0;
    }

    // SE ITERA LA MATRIZ DE PIXELES Y SE CREA EL VECTOR DE FRECUENCIAS
    for (unsigned short i = 0; i < H; i++)
    {
        for (unsigned short j = 0; j < W; j++)
        {
            unsigned short pixel = imagen_ppm.getPixel(i, j);
            frecuencias[pixel]++;
        }
    }

    // GENERACION DE NODOS
    for (int i = 0; i < 256; i++)
    {
        if (frecuencias[i] > 0)
        {
            Nodo *nuevoNodo = new Nodo();
            nuevoNodo->setSimbolo(i);
            nuevoNodo->setFrecuency(frecuencias[i]);
            mHeap.push(nuevoNodo);
        }
    }

    if (mHeap.size() == 1)
    {
        Nodo *nodo = new Nodo();
        nodo->setSimbolo(-1);
        nodo->setFrecuency(1);
        mHeap.push(nodo);
    }

    // SE RE-CONSTRUYE EL ARBOL DEPENDIENDO DE LAS FRECUENCIAS.
    while (mHeap.size() > 1)
    {
        Nodo *hijoIzq = mHeap.top();
        mHeap.pop();
        Nodo *hijoDer = mHeap.top();
        mHeap.pop();

        Nodo *padre = new Nodo();
        padre->setSimbolo(-1);
        padre->setFrecuency(hijoIzq->getFrecuency() + hijoDer->getFrecuency());
        padre->setHijoIzq(hijoIzq);
        padre->setHijoDer(hijoDer);
        mHeap.push(padre);
    }

    this->raiz = mHeap.top();

    std::multimap<int, std::string> tabla = this->construirTablaCodigos();
    this->generarArchivoBinario(imagen_ppm, tabla, archivo_huffman, frecuencias);
}

std::multimap<int, std::string> HuffmanTree::construirTablaCodigos()
{
    std::multimap<int, std::string> tablaCodigos;
    this->construirCodigos(this->raiz, "", tablaCodigos);
    return tablaCodigos;
}

void HuffmanTree::construirCodigos(Nodo *nodo, std::string cod, std::multimap<int, std::string> &tabla)
{
    if (!nodo->esHoja())
    {
        construirCodigos(nodo->getHijoIzq(), cod + "0", tabla);
        construirCodigos(nodo->getHijoDer(), cod + "1", tabla);
    }
    else
    {
        tabla.insert({nodo->getSimbolo(), cod});
    }
}

void HuffmanTree::generarArchivoBinario(Imagen &imagen_ppm, std::multimap<int, std::string> tabla, std::string archivo, unsigned long frecuencias[256])
{

    std::ofstream file(archivo, std::ios::out | std::ios::binary);

    // Se escribe el W (unsigned short)
    file.write((char *)&imagen_ppm.getW(), sizeof(unsigned short));
    // Se escribe el H (unsigned short)
    file.write((char *)&imagen_ppm.getH(), sizeof(unsigned short));
    // Se escribe el M (unsigned short)
    file.write((char *)&imagen_ppm.getM(), sizeof(unsigned short));

    for (unsigned long i = 0; i < 256; i++)
    {
        // Se escribe cada frecuencia de cada pixel posible (0-255)
        unsigned long freq = frecuencias[i];
        file.write((char *)&freq, sizeof(unsigned long));
    }

    // Se escribe una cadena de caracteres con la codificación de pixeles con el arbol (Leido por filas)
    std::string valor = "";
    for (unsigned short i = 0; i < imagen_ppm.getH(); i++)
    {
        for (unsigned short j = 0; j < imagen_ppm.getW(); j++)
        {
            unsigned short pixel = imagen_ppm.getPixel(i, j);
            std::multimap<int, std::string>::const_iterator it = tabla.find(pixel);
            std::string cod = it->second;
            valor = valor + cod;
        }
    }

    file << valor;
    file.close();
    std::cout << "\nLa imagen se codifico en un archivo de Huffman de manera exitosa";
}

void HuffmanTree::decodificarArchivo(std::string archivo_huffman, std::string archivo_ppm)
{
    std::priority_queue<Nodo *, std::vector<Nodo *>, mayorFrecuencia> mHeap;
    Imagen image;
    unsigned short W;
    unsigned short H;
    unsigned short M;
    unsigned long frecuencias[256];
    std::string codigo;

    unsigned short contW = 0;
    unsigned short contH = 0;

    for (unsigned long i = 0; i < 256; i++)
    {
        frecuencias[i] = 0;
    }

    std::ifstream in(archivo_huffman, std::ios::in | std::ios::binary);
    if (in.is_open())
    {
        in.read((char *)&W, sizeof(unsigned short));
        image.setW(W);
        // std::cout << "\nW: " << image.getW();
        in.read((char *)&H, sizeof(unsigned short));
        image.setH(H);
        // std::cout << "\nH: " << image.getH();
        in.read((char *)&M, sizeof(unsigned short));
        image.setM(M);
        // std::cout << "\nM: " << image.getM();
        for (unsigned long i = 0; i < 256; i++)
        {
            unsigned long freq;
            in.read((char *)&freq, sizeof(unsigned long));
            frecuencias[i] = freq;
        }
        in >> codigo;
        in.close();

        image.inicializar();

        for (int i = 0; i < 256; i++)
        {
            if (frecuencias[i] > 0)
            {
                Nodo *nuevoNodo = new Nodo();
                nuevoNodo->setSimbolo(i);
                nuevoNodo->setFrecuency(frecuencias[i]);
                mHeap.push(nuevoNodo);
            }
        }

        if (mHeap.size() == 1)
        {
            Nodo *nodo = new Nodo();
            nodo->setSimbolo(-1);
            nodo->setFrecuency(1);
            mHeap.push(nodo);
        }

        // SE RE-CONSTRUYE EL ARBOL DEPENDIENDO DE LAS FRECUENCIAS.
        while (mHeap.size() > 1)
        {
            Nodo *hijoIzq = mHeap.top();
            mHeap.pop();
            Nodo *hijoDer = mHeap.top();
            mHeap.pop();

            Nodo *padre = new Nodo();
            padre->setSimbolo(-1);
            padre->setFrecuency(hijoIzq->getFrecuency() + hijoDer->getFrecuency());
            padre->setHijoIzq(hijoIzq);
            padre->setHijoDer(hijoDer);
            mHeap.push(padre);
        }

        this->raiz = mHeap.top();

        std::multimap<int, std::string> tabla = this->construirTablaCodigos();

        // Descomprimir
        std::string palabraDec;
        Nodo *current = this->raiz;
        int i = 0;
        while (i < codigo.length())
        {
            while (!current->esHoja())
            {
                char bit = codigo[i];
                if (bit == '1')
                {
                    current = current->getHijoDer();
                }
                else if (bit == '0')
                {
                    current = current->getHijoIzq();
                }
                else
                {
                    std::cout << "\nCodificacion erronea";
                }
                i++;
            }

            int simbol = current->getSimbolo();
            unsigned short p = (unsigned short)simbol;
            image.setPixel(contH, contW, p);
            contW++;

            if (contW == image.getW())
            {
                contW = 0;
                contH++;
            }

            current = this->raiz;
        }

        std::cout << "\nFinalize la construcción de la imagen";
        this->generarImagenPGM(image, archivo_ppm);
    }
    else
    {
        std::cout << "\nArchivo invalido";
    }
}

void HuffmanTree::generarImagenPGM(Imagen &imagen_ppm, std::string archivo_ppm)
{
    unsigned short X = imagen_ppm.getH();
    unsigned short Y = imagen_ppm.getW();
    bool primero = false;

    std::ofstream output;
    output.open(archivo_ppm, std::ios::out);
    output << "P2" << std::endl;
    output << Y << " " << X << std::endl;
    output << imagen_ppm.getM() << std::endl;
    for (unsigned short a = 0; a < X; a++)
    {
        if (primero)
            output << std::endl;
        for (unsigned short z = 0; z < Y; z++)
        {
            unsigned short actual = imagen_ppm.getPixel(a, z);
            output << actual << " ";
            primero = true;
        }
    }
    output.close();
    std::cout << "\nLa imagen decodificada se creo y guardó exitosamente";
}
