#ifndef HUFFMANTREE_H_
#define HUFFMANTREE_H_

#include <iostream>
#include <string>
#include <queue>
#include <map>
#include "Nodo.h"
#include "Imagen.h"

class HuffmanTree
{

protected:
    Nodo *raiz;

public:
    HuffmanTree();
    void construirArbol(Imagen &imagen_ppm, std::string archivo_huffman);
    std::multimap<int, std::string> construirTablaCodigos();
    void construirCodigos(Nodo *nodo, std::string cod, std::multimap<int, std::string> &tabla);
    void generarArchivoBinario(Imagen &imagen_ppm, std::multimap<int, std::string> tabla, std::string archivo, unsigned long frecuencias[256]);
    void decodificarArchivo(std::string archivo_huffman, std::string archivo_ppm);
    void generarImagenPGM(Imagen &imagen_ppm, std::string archivo_ppm);
};

#include "huffmanTree.cpp"

#endif