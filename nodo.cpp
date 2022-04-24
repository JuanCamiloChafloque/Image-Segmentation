#include <iostream>
#include <sstream>
#include <queue>
#include <map>
#include <vector>
#include "Nodo.h"

Nodo::Nodo()
{
    this->hijoDer = NULL;
    this->hijoIzq = NULL;
}

const int &Nodo::getSimbolo() const
{
    return this->simbolo;
}

const unsigned short &Nodo::getFrecuency() const
{
    return this->frecuency;
}

void Nodo::setSimbolo(int pSimbolo)
{
    this->simbolo = pSimbolo;
}

void Nodo::setFrecuency(unsigned short pFrecuency)
{
    this->frecuency = pFrecuency;
}

void Nodo::setHijoIzq(Nodo *hijoIzq)
{
    this->hijoIzq = hijoIzq;
}

void Nodo::setHijoDer(Nodo *hijoDer)
{
    this->hijoDer = hijoDer;
}

Nodo *Nodo::getHijoIzq()
{
    return this->hijoIzq;
}

Nodo *Nodo::getHijoDer()
{
    return this->hijoDer;
}

bool Nodo::esHoja()
{
    return (this->hijoDer == NULL && this->hijoIzq == NULL);
}